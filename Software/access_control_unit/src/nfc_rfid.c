/**
 * \file        nfc_rfid.c
 * \brief
 * \details
 * \author      MST_CDA
 * \version     0.0.1
 * \date        05/10/2023
 * \copyright   Unlicensed
 */

#include "nfc_rfid.h"

void nfc_init_as_spi(nfc_rfid_t *nfc, spi_inst_t *_spi, uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs, uint8_t irq, uint8_t rst)
{
    nfc->spi = _spi;
    nfc->pinout.sck = sck;
    nfc->pinout.mosi = mosi;
    nfc->pinout.miso = miso;
    nfc->pinout.cs = cs;
    nfc->pinout.irq = irq;
    nfc->pinout.rst = rst;
    nfc->userType = NONE;
    nfc->timeCheck = 1000000; ///< 1s = 1000000 us
    nfc->timer_irq = TIMER_IRQ_1;
    nfc->blockAddr = 1;
    nfc->sizeRead = 18;
	nfc->tag.is_present = false;
	nfc->check = true;

    nfc->spi = _spi;
    if (_spi == spi0){
        nfc->spi_irq = SPI0_IRQ;
    } else if (_spi == spi1){
        nfc->spi_irq = SPI1_IRQ;
    }
    // Initialize the SPI buffer
    for (int i = 0; i < BUFFER_SIZE; i++) {
		nfc->Rx_Buf[i] = 0;
		nfc->Tx_Buf[i] = 0;
	}

    // Reset configuration
    gpio_init(rst);
    // gpio_set_dir(rst, GPIO_IN);
    gpio_put(rst, 0);
    sleep_ms(1000);
    gpio_put(rst, 1);
	sleep_ms(50);

    // Chip select is active-low, so we'll initialise it to a driven-high state
    gpio_init(cs);
    gpio_set_dir(cs, GPIO_OUT);
    gpio_put(cs, 1); ///< Set the CS pin to high

    // Configuring the ARM Primecell Synchronous Serial Port (SSP)
    spi_init(_spi, 1000000); ///< Initialize the SPI bus with a speed of 1 Mbps
    spi_set_format(_spi, 8, 0, 0, SPI_MSB_FIRST);
    gpio_set_function(sck,  GPIO_FUNC_SPI);
    gpio_set_function(mosi, GPIO_FUNC_SPI);
    gpio_set_function(miso, GPIO_FUNC_SPI);

    nfc_write(nfc, CommandReg, PCD_SoftReset); // Perform a soft reset

    // // Reset baud rates
    // nfc_write(nfc, TxModeReg, 0x00);
    // nfc_write(nfc, RxModeReg, 0x00);
    // // Reset ModWidthReg
    // nfc_write(nfc, ModWidthReg, 0x26);

    // When communicating with a PICC we need a timeout if something goes wrong.
	// f_timer = 13.56 MHz / (2*TPreScaler+1) where TPreScaler = [TPrescaler_Hi:TPrescaler_Lo].
	// TPrescaler_Hi are the four low bits in TModeReg. TPrescaler_Lo is TPrescalerReg.
    nfc_write(nfc, TModeReg, 0x80); ///< TAuto=1; timer starts automatically at the end of the transmission in all communication modes at all speeds
    nfc_write(nfc, TPrescalerReg, 0xA9); // TPreScaler = TModeReg[3..0]:TPrescalerReg, ie 0x0A9 = 169 => f_timer=40kHz, ie a timer period of 25μs.
    nfc_write(nfc, TReloadRegH, 0x03); ///< Reload timer with 0x3E8 = 1000, ie 25ms before timeout.
    nfc_write(nfc, TReloadRegL, 0xE8);

    nfc_write(nfc, TxASKReg, 0x40); ///< Default 0x00. Force a 100 % ASK modulation independent of the ModGsPReg register setting
    nfc_write(nfc, ModeReg, 0x3D); // Default 0x3F. Set the preset value for the CRC coprocessor for the CalcCRC command to 0x6363 (ISO 14443-3 part 6.2.4)
    
    nfc_antenna_on(nfc); ///< Enable the antenna

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (uint8_t i = 0; i < MF_KEY_SIZE; i++) {
        nfc->keyByte[i] = 0xFF;
    }

    ///< Initialize the uid data base
	nfc->uid_data_base[0] = 0xc64eacf8;
	nfc->uid_data_base[1] = 0x1b541c5f;
	nfc_load_last_uid(nfc);

}

bool nfc_is_new_tag(nfc_rfid_t *nfc)
{
    uint8_t bufferATQA[2];
	uint8_t bufferSize = sizeof(bufferATQA);

    StatusCode result = nfc_requestA(nfc, bufferATQA, &bufferSize);

    return (result == STATUS_OK || result == STATUS_COLLISION);
}

uint8_t nfc_authenticate(nfc_rfid_t *nfc, uint8_t command, uint8_t blockAddr, uint8_t *keyByte, Uid *uid)
{
    uint8_t waitIRq = 0x10; // IdleIRq

	// Build command buffer
	uint8_t sendData[12];
	sendData[0] = command;
	sendData[1] = blockAddr;
	for (uint8_t i = 0; i < MF_KEY_SIZE; i++) { // 6 key uint8_ts
		sendData[2 + i] = keyByte[i];
	}
	for (uint8_t i = 0; i < 4; i++) { // The first 4 uint8_ts of the UID
		sendData[8 + i] = nfc->uid.uidByte[i];
	}
    StatusCode status = nfc_communicate(nfc, PCD_MFAuthent, waitIRq, &sendData[0], sizeof(sendData), NULL, 0, 0, 0, false);
    return status;
}

uint8_t nfc_select(nfc_rfid_t *nfc, Uid *uid, uint8_t validBits)
{	
    bool uidComplete;
	bool selectDone;
	bool useCascadeTag;
	uint8_t cascadeLevel = 1;
	StatusCode result;
	uint8_t count;
	uint8_t index;
	uint8_t uidIndex; // The first index in uid->uiduint8_t[] that is used in
					  // the current Cascade Level.
	int8_t currentLevelKnownBits; // The number of known UID bits in the current
								  // Cascade Level.
	uint8_t buffer[9];  // The SELECT/ANTICOLLISION commands uses a 7 uint8_t
						// standard frame + 2 uint8_ts CRC_A
	uint8_t bufferUsed; // The number of uint8_ts used in the buffer, ie the
						// number of uint8_ts to transfer to the FIFO.
	uint8_t rxAlign; // Used in BitFramingReg. Defines the bit position for the
					 // first bit received.
	uint8_t txLastBits; // Used in BitFramingReg. The number of valid bits in
						// the last transmitted uint8_t.
	uint8_t *responseBuffer;
	uint8_t responseLength;

	// Description of buffer structure:
	//		uint8_t 0: SEL				Indicates the Cascade Level: PICC_CMD_SEL_CL1,
	//PICC_CMD_SEL_CL2 or PICC_CMD_SEL_CL3
	//		uint8_t 1: NVB					Number of Valid Bits (in complete command, not
	//just the UID): High nibble: complete uint8_ts, Low nibble: Extra bits.
	//		uint8_t 2: UID-data or CT		See explanation below. CT means Cascade
	//Tag.
	//		uint8_t 3: UID-data
	//		uint8_t 4: UID-data
	//		uint8_t 5: UID-data
	//		uint8_t 6: BCC					Block Check Character - XOR of uint8_ts
	//2-5
	//		uint8_t 7: CRC_A
	//		uint8_t 8: CRC_A
	// The BCC and CRC_A are only transmitted if we know all the UID bits of the
	// current Cascade Level.
	//
	// Description of uint8_ts 2-5: (Section 6.5.4 of the ISO/IEC 14443-3 draft:
	// UID contents and cascade levels)
	//		UID size	Cascade level	uint8_t2	uint8_t3	uint8_t4
	//uint8_t5
	//		========	=============	=====	=====	=====	=====
	//		 4 uint8_ts		1			uid0	uid1	uid2	uid3
	//		 7 uint8_ts		1			CT		uid0	uid1	uid2
	//						2			uid3	uid4	uid5	uid6
	//		10 uint8_ts		1			CT		uid0	uid1	uid2
	//						2			CT		uid3	uid4	uid5
	//						3			uid6	uid7	uid8	uid9

	// Sanity checks
	if (validBits > 80) {
		return STATUS_INVALID;
	}

	// Prepare MFRC522
	nfc_clear_reg_bitmask(nfc, CollReg, 0x80); // ValuesAfterColl=1 => Bits
												   // received after collision
												   // are cleared.

	// Repeat Cascade Level loop until we have a complete UID.
	uidComplete = false;
	while (!uidComplete) {
		// Set the Cascade Level in the SEL uint8_t, find out if we need to use
		// the Cascade Tag in uint8_t 2.
		switch (cascadeLevel) {
		case 1:
			buffer[0] = PICC_CMD_SEL_CL1;
			uidIndex = 0;
			useCascadeTag = validBits && uid->size > 4; // When we know that the UID has more than 4 uint8_ts
			break;

		case 2:
			buffer[0] = PICC_CMD_SEL_CL2;
			uidIndex = 3;
			useCascadeTag = validBits && uid->size > 7; // When we know that the UID has more than 7 uint8_ts
			break;

		case 3:
			buffer[0] = PICC_CMD_SEL_CL3;
			uidIndex = 6;
			useCascadeTag = false; // Never used in CL3.
			break;

		default:
			return STATUS_INTERNAL_ERROR;
			break;
		}

		// How many UID bits are known in this Cascade Level?
		currentLevelKnownBits = validBits - (8 * uidIndex);
		if (currentLevelKnownBits < 0) {
			currentLevelKnownBits = 0;
		}
		// Copy the known bits from uid->uiduint8_t[] to buffer[]
		index = 2; // destination index in buffer[]
		if (useCascadeTag) {
			buffer[index++] = PICC_CMD_CT;
		}
		uint8_t uint8_tsToCopy = currentLevelKnownBits / 8 + (currentLevelKnownBits % 8 ? 1 : 0); // The number of uint8_ts
												 // needed to represent the
												 // known bits for this level.
		if (uint8_tsToCopy) {
			uint8_t maxuint8_ts = useCascadeTag ? 3 : 4; // Max 4 uint8_ts in each Cascade Level.
									   // Only 3 left if we use the Cascade Tag
			if (uint8_tsToCopy > maxuint8_ts) {
				uint8_tsToCopy = maxuint8_ts;
			}
			for (count = 0; count < uint8_tsToCopy; count++) {
				buffer[index++] = uid->uidByte[uidIndex + count];
			}
		}
		// Now that the data has been copied we need to include the 8 bits in CT
		// in currentLevelKnownBits
		if (useCascadeTag) {
			currentLevelKnownBits += 8;
		}

		// Repeat anti collision loop until we can transmit all UID bits + BCC
		// and receive a SAK - max 32 iterations.
		selectDone = false;
		while (!selectDone) {
			// Find out how many bits and uint8_ts to send and receive.
			if (currentLevelKnownBits >= 32) { // All UID bits in this Cascade
											   // Level are known. This is a
											   // SELECT.
				// printf(F("SELECT: currentLevelKnownBits="));
				// printf(currentLevelKnownBits, DEC);
				buffer[1] = 0x70; // NVB - Number of Valid Bits: Seven whole uint8_ts
				// Calculate BCC - Block Check Character
				buffer[6] = buffer[2] ^ buffer[3] ^ buffer[4] ^ buffer[5];
				// Calculate CRC_A
				result = nfc_calculate_crc(nfc, buffer, 7, &buffer[7]);
				if (result != STATUS_OK) {
					return result;
				}
				txLastBits = 0; // 0 => All 8 bits are valid.
				bufferUsed = 9;
				// Store response in the last 3 uint8_ts of buffer (BCC and
				// CRC_A - not needed after tx)
				responseBuffer = &buffer[6];
				responseLength = 3;
			} else { // This is an ANTICOLLISION.
				// printf(F("ANTICOLLISION: currentLevelKnownBits="));
				// printf(currentLevelKnownBits, DEC);
				txLastBits = currentLevelKnownBits % 8;
				count = currentLevelKnownBits / 8;		   // Number of whole uint8_ts in the UID part.
				index = 2 + count; // Number of whole uint8_ts: SEL + NVB + UIDs
				buffer[1] = (index << 4) + txLastBits; // NVB - Number of Valid Bits
				bufferUsed = index + (txLastBits ? 1 : 0);
				// Store response in the unused part of buffer
				responseBuffer = &buffer[index];
				responseLength = sizeof(buffer) - index;
			}

			// Set bit adjustments
			rxAlign = txLastBits; // Having a separate variable is overkill. But
								  // it makes the next line easier to read.
			nfc_write(nfc, BitFramingReg, (rxAlign << 4) + txLastBits); // RxAlign = BitFramingReg[6..4].
											   // TxLastBits =
											   // BitFramingReg[2..0]

			// Transmit the buffer and receive the response.
			result = nfc_transceive_data(nfc, buffer, bufferUsed, responseBuffer, &responseLength, &txLastBits, rxAlign, false);
			if (result == STATUS_COLLISION) { // More than one PICC in the field
											  // => collision.
				uint8_t valueOfCollReg = nfc_read(nfc, CollReg); // CollReg[7..0] bits are: ValuesAfterColl
									// reserved CollPosNotValid CollPos[4:0]
				if (valueOfCollReg & 0x20) { // CollPosNotValid
					return STATUS_COLLISION; // Without a valid collision position we cannot continue
				}
				uint8_t collisionPos = valueOfCollReg & 0x1F; // Values 0-31, 0 means bit 32.
				if (collisionPos == 0) {
					collisionPos = 32;
				}
				if (collisionPos <= currentLevelKnownBits) { // No progress - should not happen
					return STATUS_INTERNAL_ERROR;
				}
				// Choose the PICC with the bit set.
				currentLevelKnownBits = collisionPos;
				count = (currentLevelKnownBits - 1) % 8; // The bit to modify
				index = 1 + (currentLevelKnownBits / 8) + (count ? 1 : 0); // First uint8_t is index 0.
				buffer[index] |= (1 << count);
			} else if (result != STATUS_OK) {
				return result;
			} else {							   // STATUS_OK
				if (currentLevelKnownBits >= 32) { // This was a SELECT.
					selectDone = true;			   // No more anticollision
					// We continue below outside the while.
				} else { // This was an ANTICOLLISION.
					// We now have all 32 bits of the UID in this Cascade Level
					currentLevelKnownBits = 32;
					// Run loop again to do the SELECT.
				}
			}
		} // End of while (!selectDone)

		// We do not check the CBB - it was constructed by us above.

		// Copy the found UID uint8_ts from buffer[] to uid->uiduint8_t[]
		index = (buffer[2] == PICC_CMD_CT) ? 3 : 2; // source index in buffer[]
		uint8_tsToCopy = (buffer[2] == PICC_CMD_CT) ? 3 : 4;
		for (count = 0; count < uint8_tsToCopy; count++) {
			uid->uidByte[uidIndex + count] = buffer[index++];
		}

		// Check response SAK (Select Acknowledge)
		if (responseLength != 3 ||
			txLastBits !=
				0) { // SAK must be exactly 24 bits (1 uint8_t + CRC_A).
			return STATUS_ERROR;
		}
		// Verify CRC_A - do our own calculation and store the control in
		// buffer[2..3] - those uint8_ts are not needed anymore.
		result = nfc_calculate_crc(nfc, responseBuffer, 1, &buffer[2]);
		if (result != STATUS_OK) {
			return result;
		}
		if ((buffer[2] != responseBuffer[1]) ||
			(buffer[3] != responseBuffer[2])) {
			return STATUS_CRC_WRONG;
		}
		if (responseBuffer[0] & 0x04) { // Cascade bit set - UID not complete yes
			cascadeLevel++;
		} else {
			uidComplete = true;
			uid->sak = responseBuffer[0];
		}
	} // End of while (!uidComplete)

	// Set correct uid->size
	uid->size = 3 * cascadeLevel + 1;

	return STATUS_OK;
} // End of nfc_select

uint8_t nfc_read_card(nfc_rfid_t *nfc, uint8_t blockAddr, uint8_t *buffer, uint8_t *bufferSize)
{
    StatusCode result;

	// Sanity check
	if (buffer == NULL || *bufferSize < 18) {
		return STATUS_NO_ROOM;
	}

	// Build command buffer
	buffer[0] = PICC_CMD_MF_READ;
	buffer[1] = blockAddr;
	// Calculate CRC_A
	result = nfc_calculate_crc(nfc, buffer, 2, &buffer[2]);
	if (result != STATUS_OK) {
		return result;
	}

	// Transmit the buffer and receive the response, validate CRC_A.
	return nfc_transceive_data(nfc, buffer, 4, buffer, bufferSize, NULL, 0, true);
}

uint8_t nfc_communicate(nfc_rfid_t *nfc, uint8_t command, uint8_t waitIRq, uint8_t *sendData, uint8_t sendLen, 
                        uint8_t *backData, uint8_t *backLen, uint8_t *validBits, uint8_t rxAlign, bool checkCRC)
{
    // Prepare values for BitFramingReg
    uint8_t txLastBits = validBits ? *validBits : 0;
    uint8_t bitFraming = (rxAlign << 4) + txLastBits; // RxAlign = BitFramingReg[6..4]. TxLastBits = BitFramingReg[2..0]

    nfc_write(nfc, CommandReg, PCD_Idle);  // Stop any active command.
    nfc_write(nfc, ComIrqReg, 0x7F);       // Clear all seven interrupt request bits
    nfc_write(nfc, FIFOLevelReg, 0x80);    // FlushBuffer = 1, FIFO initialization
    nfc_write_mult(nfc, FIFODataReg, sendData, sendLen); // Write sendData to the FIFO
    nfc_write(nfc, BitFramingReg, bitFraming); // Bit adjustments
    nfc_write(nfc, CommandReg, command); // Execute the command
    if (command == PCD_Transceive) {
        nfc_set_reg_bitmask(nfc, BitFramingReg, 0x80); // StartSend=1, transmission of data starts
    }

    // In PCD_Init() we set the TAuto flag in TModeReg. This means the timer
	// automatically starts when the PCD stops transmitting.

    uint8_t n;
	unsigned int i;

    // Wait for the command to complete.
	// In PCD_Init() we set the TAuto flag in TModeReg. This means the timer
	// automatically starts when the PCD stops transmitting.
	// Each iteration of the do-while-loop takes 17.86us.
	i = 2000;
	while (1) {
		n = nfc_read(nfc, ComIrqReg); // ComIrqReg[7..0] bits are: Set1
											   // TxIRq RxIRq IdleIRq HiAlertIRq
											   // LoAlertIRq ErrIRq TimerIRq
		if (n & waitIRq) { // One of the interrupts that signal success has been
						   // set.
			break;
		}
		if (n & 0x01) { // Timer interrupt - nothing received in 25ms
			return STATUS_TIMEOUT;
		}
		if (--i == 0) { // The emergency break. If all other conditions fail we
						// will eventually terminate on this one after 35.7ms.
						// Communication with the MFRC522 might be down.
			return STATUS_TIMEOUT;
		}
	}

    // Stop now if any errors except collisions were detected.
    uint8_t errorRegValue = nfc_read(nfc, ErrorReg); ///< ErrorReg[7..0] bits are: WrErr TempErr reserved BufferOvfl CollErr CRCErr ParityErr ProtocolErr

    if (errorRegValue & 0x13) { ///< BufferOvfl ParityErr ProtocolErr
        return STATUS_ERROR;
    }

    uint8_t _validBits = 0;

    // If the caller wants data back, get it from the MFRC522.
    if (backData && backLen) {
        n = nfc_read(nfc, FIFOLevelReg); ///< Number of bytes in the FIFO
        if (n > *backLen) {
            return STATUS_NO_ROOM;
        }
        *backLen = n; ///< Number of bytes returned
        nfc_read_mult(nfc, FIFODataReg, backData, n, rxAlign); ///< Get received data from FIFO
        _validBits = nfc_read(nfc, ControlReg) & 0x07; ///< RxLastBits[2:0] indicates the number of valid bits in the last received byte. If this value is 000b, the whole byte is valid
        if (validBits) {
            *validBits = _validBits;
        }
    } 

    // Tell about collisions
    if (errorRegValue & 0x08) { ///< CollErr
        return STATUS_COLLISION;
    }

    // Perform CRC_A validation if requested.
    if (backData && backLen && checkCRC) {
        // In this case a MIFARE Classic NAK is not OK.
        if (*backLen == 1 && _validBits == 4) {
            return STATUS_MIFARE_NACK;
        }
        // We need at least the CRC_A value and all 8 bits of the last byte must be received.
        if (*backLen < 2 || _validBits != 0) {
            return STATUS_CRC_WRONG;
        }
        // Verify CRC_A - do our own calculation and store the control in controlBuffer.
        uint8_t controlBuffer[2];
        StatusCode status = nfc_calculate_crc(nfc, &backData[0], *backLen - 2, &controlBuffer[0]); // (it is not done)
        if (status != STATUS_OK) {
            return status;
        }
        if ((backData[*backLen - 2] != controlBuffer[0]) || (backData[*backLen - 1] != controlBuffer[1])) {
            return STATUS_CRC_WRONG;
        }
    }

    return STATUS_OK;
} // End of nfc_communicate

uint8_t nfc_requestA_or_wakeupA(nfc_rfid_t *nfc, uint8_t command, uint8_t *bufferATQA, uint8_t *bufferSize)
{
    uint8_t validBits;
    StatusCode status;

    if (bufferATQA == NULL || *bufferSize < 2) { // The ATQA response is 2 bytes long.
        return STATUS_NO_ROOM;
    }
    nfc_clear_reg_bitmask(nfc, CollReg, 0x80); // ValuesAfterColl=1 => Bits received after collision are cleared.
    validBits = 7;
    status = nfc_transceive_data(nfc, &command, 1, bufferATQA, bufferSize, &validBits, 0, false);
    if (status != STATUS_OK) {
        return status;
    }
    if (*bufferSize != 2 || validBits != 0) { // ATQA must be exactly 16 bits.
        return STATUS_ERROR;
    }

    return STATUS_OK;
} // End of nfc_requestA_or_wakeupA

uint8_t nfc_calculate_crc(nfc_rfid_t *nfc, uint8_t *data, uint8_t len, uint8_t *result)
{
    nfc_write(nfc, CommandReg, PCD_Idle); // Stop any active command.
	nfc_write(nfc, DivIrqReg, 0x04); // Clear the CRCIRq interrupt request bit
	nfc_set_reg_bitmask(nfc, FIFOLevelReg, 0x80); // FlushBuffer = 1, FIFO initialization
	nfc_write_mult(nfc, FIFODataReg, data, len); // Write data to the FIFO
	nfc_write(nfc, CommandReg, PCD_CalcCRC); // Start the calculation

	// Wait for the CRC calculation to complete. Each iteration of the
	// while-loop takes 17.73�s.
	uint16_t i = 5000;
	uint8_t n;
	while (1) {
		n = nfc_read(nfc, DivIrqReg); // DivIrqReg[7..0] bits are: Set2 reserved reserved MfinActIRq reserved CRCIRq reserved reserved
		if (n & 0x04) { // CRCIRq bit set - calculation done
			break;
		}
		if (--i == 0) { // The emergency break. We will eventually terminate on
						// this one after 89ms. Communication with the MFRC522
						// might be down.
			return STATUS_TIMEOUT;
		}
	}
	nfc_write(nfc, CommandReg, PCD_Idle); // Stop calculating CRC for new content in the FIFO.

	// Transfer the result from the registers to the result buffer
	result[0] = nfc_read(nfc, CRCResultRegL);
	result[1] = nfc_read(nfc, CRCResultRegH);
	return STATUS_OK;
} // End of nfc_calculate_crc

bool nfc_get_data_tag(nfc_rfid_t *nfc)
{
    // The first byte of bufferRead is the product ID.
    nfc->tag.id = nfc->bufferRead[15];
	printf("ID: %02x\n", nfc->tag.id);

	// From the ID, we can determine the type of the user
	if (nfc->tag.id == 0x07) {
		nfc->userType = ADMIN;
	} else if (nfc->tag.id == 0x06) {
		nfc->userType = INV;
	} else if (nfc->tag.id == 0x01 || nfc->tag.id == 0x02 || nfc->tag.id == 0x03 || nfc->tag.id == 0x04 || nfc->tag.id == 0x05) {
		nfc->userType = USER;
		nfc->tag.amount = (nfc->bufferRead[11] << 24) | (nfc->bufferRead[12] << 16) | (nfc->bufferRead[13] << 8) | nfc->bufferRead[14];
		nfc->tag.purchase_v = (nfc->bufferRead[7] << 24) | (nfc->bufferRead[8] << 16) | (nfc->bufferRead[9] << 8) | nfc->bufferRead[10];
		nfc->tag.sale_v = (nfc->bufferRead[3] << 24) | (nfc->bufferRead[4] << 16) | (nfc->bufferRead[5] << 8) | nfc->bufferRead[6];

		if ((nfc->tag.amount >> 31) | (nfc->tag.purchase_v >> 31) | (nfc->tag.sale_v >> 31)) { ///< Negative values
			nfc->tag.is_present = false;
			return false;
		}
		printf("Amount: %u\n", nfc->tag.amount);
		printf("Purchase value: %u\n", nfc->tag.purchase_v);
		printf("Sale value: %u\n", nfc->tag.sale_v);
	} else { ///< Invalid ID
		nfc->tag.is_present = false;
		return false;
	}

	return true;
} // End of nfc_get_data_tag

bool nfc_check_is_valid_tag(nfc_rfid_t *nfc)
{
	nfc_read_card_serial(nfc); ///< Read the serial number of the card
	// Print the serial number of the card
	printf("\nCard UID: ");
	uint32_t uid = 0x00;
	for (int i = 0; i < nfc->uid.size; i++) {
		printf("%02X", nfc->uid.uidByte[i]);
		uid = (uid << 8) | nfc->uid.uidByte[i];
	}
	printf("\n");

	// Check if the card is in the data base
	nfc->tag.uid = uid;
	for (uint8_t i = 0; i < SIZE_UID_DATA_BASE; i++) {
		if (uid == nfc->uid_data_base[i]) {
			nfc->tag.is_present = true;
			nfc->tag.uid_reg_access = 0b1;
			nfc_store_last_uid(nfc); ///< Store the last UID in the flash
			return true;
		}
	}

	nfc->tag.is_present = false;
	nfc->tag.uid_reg_access = 0b0;
	nfc_store_last_uid(nfc); ///< Store the last UID in the flash
	return false;
}

void nfc_store_last_uid(nfc_rfid_t *nfc)
{
	// An array of 256 bytes, multiple of FLASH_PAGE_SIZE. Database is 60 bytes.
    uint32_t buf[FLASH_PAGE_SIZE/sizeof(uint32_t)];

    // Copy the database into the buffer
    buf[0] = nfc->tag.uid;
	buf[1] = (uint32_t)nfc->tag.uid_reg_access;

    // Program buf[] into the first page of this sector
    // Each page is 256 bytes, and each sector is 4K bytes
    // Erase the last sector of the flash
    flash_safe_execute(nfc_flash_wrapper, NULL, 500);

    uint32_t ints = save_and_disable_interrupts();
    flash_range_program(FLASH_TARGET_OFFSET, (uint8_t *)buf, FLASH_PAGE_SIZE);
    restore_interrupts (ints);
}

void nfc_load_last_uid(nfc_rfid_t *nfc)
{
	// Compute the memory-mapped address, remembering to include the offset for RAM
    uint32_t addr = XIP_BASE + FLASH_TARGET_OFFSET;
    uint32_t *ptr = (uint32_t *)addr; ///< Place an int pointer at our memory-mapped address

    nfc->tag.uid = ptr[0];
	if ((uint8_t)ptr[1] != 0b0 && (uint8_t)ptr[1] != 0b1) {
		nfc->tag.uid_reg_access = 0b0;
	} else {
		nfc->tag.uid_reg_access = (uint8_t)ptr[1];
	}
}
