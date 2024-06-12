/**
 * \file        nfc_rfid.h
 * \brief
 * \details
 * \author      MST_CDA
 * \version     0.0.1
 * \date        05/10/2023
 * \copyright   Unlicensed
 */

#ifndef __NFC_RFID_
#define __NFC_RFID_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/time.h"
#include "hardware/spi.h"
#include "hardware/irq.h"
#include "hardware/gpio.h"

#include "nfc_enums.h"

#define ADDRESS_SLAVE_MFRC522 0x28  ///< 0b0101 -> 0010 1000
#define MF_KEY_SIZE             6	///< A Mifare Crypto1 key is 6 bytes.
#define READ_BIT 0x80 ///< Bit used in I2C to read a register
#define BUFFER_SIZE  1 ///< Buffer size for the SPI communication
#define SIZE_UID_DATA_BASE 5 ///< Size of the UID data base

/**
 * \typedef nfc_rfic_t
 * \brief Data strcuture to manage the NFC RFID device.
 */
typedef struct
{
    struct {
        uint8_t irq;
        uint8_t rst;
        uint8_t sck;
        uint8_t mosi;
        uint8_t miso;
        uint8_t cs;
    }pinout;
	
    uint8_t keyByte[MF_KEY_SIZE]; ///< Mifare Crypto1 key	
    uint32_t timeCheck; ///< Time check (1s)
    bool check; ///< Check flag
    uint8_t timer_irq; ///< Alarm timer IRQ number (TIMER_IRQ_1)

    uint8_t spi_irq; ///< SPI IRQ number (25 or 26)
    spi_inst_t *spi; ///< SPI instance

    uint8_t Tx_Buf[BUFFER_SIZE];
	uint8_t Rx_Buf[BUFFER_SIZE];
    Uid uid; ///< UID of the tag
    tag_t tag; ///< Tag data

    uint8_t bufferRead[18]; ///< Buffer for the read data
    uint8_t sizeRead; ///< Size of the read data
    uint8_t blockAddr; ///< Block address

    enum {NONE, ADMIN, INV, USER} userType;

    uint32_t uid_data_base[SIZE_UID_DATA_BASE]; ///< Data base of the UID
    
}nfc_rfid_t;


/**
 * @brief This function initializes the nfc_rfid_t structure as SPI
 * 
 * @param nfc 
 * @param _spi 
 * @param sck 
 * @param mosi 
 * @param miso 
 * @param cs 
 * @param irq 
 * @param rst 
 */
void nfc_init_as_spi(nfc_rfid_t *nfc, spi_inst_t *_spi, uint8_t sck, uint8_t mosi, uint8_t miso, uint8_t cs, uint8_t irq, uint8_t rst);

/**
 * @brief This function tell us if there is a new tag in the NFC.
 * Returns true if a PICC responds to PICC_CMD_REQA.
 * Only "new" cards in state IDLE are invited. Sleeping cards in state HALT are ignored.
 * 
 * @param nfc 
 * @return true 
 * @return false 
 */
bool nfc_is_new_tag(nfc_rfid_t *nfc);

/**
 * @brief 
 * Executes the MFRC522 MFAuthent command.
 * This command manages MIFARE authentication to enable a secure communication to any MIFARE Mini, MIFARE 1K and MIFARE 4K card.
 * 
 * @param nfc 
 * @param command 
 * @param blockAddr 
 * @param keyByte ///< keyByte array of the NFC structure
 * @param uid 
 * @return StatusCode 
 */
uint8_t nfc_authenticate(nfc_rfid_t *nfc, uint8_t command, uint8_t blockAddr, uint8_t *keyByte, Uid *uid);

/**
 * @brief Transmits SELECT/ANTICOLLISION commands to select a single PICC.
 * Before calling this function the PICCs must be placed in the READY(*) state
 * by calling PICC_RequestA() or PICC_WakeupA().
 * 
 * @param nfc 
 * @param uid 
 * @param validBits 
 * @return StatusCode 
 */
uint8_t nfc_select(nfc_rfid_t *nfc, Uid *uid, uint8_t validBits);

/**
 * @brief Reads a block of data from the active PICC.
 * 
 * @param nfc 
 * @param blockAddr 
 * @param buffer 
 * @param bufferSize 
 * @return StatusCode 
 */
uint8_t nfc_read_card(nfc_rfid_t *nfc, uint8_t blockAddr, uint8_t *buffer, uint8_t *bufferSize);

/**
 * @brief Simple wrapper around PICC_Select.
 * Returns true if a UID could be read.
 * Remember to call PICC_IsNewCardPresent(), PICC_RequestA() or PICC_WakeupA() first. 
 * The read UID is available in the class variable uid.
 * 
 * @param nfc 
 * @return true 
 * @return false 
 */
static inline bool nfc_read_card_serial(nfc_rfid_t *nfc)
{
    StatusCode status = nfc_select(nfc, &nfc->uid, 0);
    return (status == STATUS_OK);
}

/**
 * @brief Transfers data to the MFRC522 FIFO, executes a command, waits for completion and transfers data back from the FIFO.
 * CRC validation can only be done if backData and backLen are specified.
 * 
 * @param nfc 
 * @param command   ///< The command to execute. One of the PCD_Command enums.
 * @param waitIRq   ///< The bits in the ComIrqReg register that signals successful completion of the command.
 * @param sendData  ///< Pointer to the data to transfer to the FIFO.
 * @param sendLen   ///< Number of bytes to transfer to the FIFO.
 * @param backData  ///< nullptr or pointer to buffer if data should be read back after executing the command.
 * @param backLen   ///< In: Max number of bytes to write to *backData. Out: The number of bytes returned.
 * @param validBits ///< In/Out: The number of valid bits in the last byte. 0 for 8 valid bits.
 * @param rxAlign   ///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
 * @param checkCRC  ///< In: True => The last two bytes of the response is assumed to be a CRC_A that must be validated.
 * @return uint8_t  ///< STATUS_OK on success, STATUS_??? otherwise.
 */
uint8_t nfc_communicate(nfc_rfid_t *nfc, uint8_t command, uint8_t waitIRq, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, 
                            uint8_t *backLen, uint8_t *validBits, uint8_t rxAlign, bool checkCRC);

/**
 * @brief Executes the Transceive command.
 * CRC validation can only be done if backData and backLen are specified.
 * 
 * @param nfc 
 * @param sendData  ///< Pointer to the data to transfer to the FIFO.
 * @param sendLen   ///< Number of bytes to transfer to the FIFO.
 * @param backData  ///< nullptr or pointer to buffer if data should be read back after executing the command.
 * @param backLen   ///< In: Max number of bytes to write to *backData. Out: The number of bytes returned.
 * @param validBits ///< In/Out: The number of valid bits in the last byte. 0 for 8 valid bits. Default nullptr.
 * @param rxAlign   ///< In: Defines the bit position in backData[0] for the first bit received. Default 0.
 * @param checkCRC  ///< In: True => The last two bytes of the response is assumed to be a CRC_A that must be validated.
 * @return uint8_t 
 */
static inline uint8_t nfc_transceive_data(nfc_rfid_t *nfc, uint8_t *sendData, uint8_t sendLen, uint8_t *backData, 
                            uint8_t *backLen, uint8_t *validBits, uint8_t rxAlign, bool checkCRC)
{
    uint8_t waitIRq = 0x30; // RxIRq and IdleIRq
    return nfc_communicate(nfc, (uint8_t)PCD_Transceive, waitIRq, sendData, sendLen, backData, backLen, validBits, rxAlign, checkCRC);
}

/**
 * @brief Transmits REQA or WUPA commands.
 * 
 * @param nfc 
 * @param command ///< The command to send - PICC_CMD_REQA or PICC_CMD_WUPA
 * @param bufferATQA ///< The buffer to store the ATQA (Answer to request) in
 * @param bufferSize ///< Buffer size, at least two bytes. Also number of bytes returned if STATUS_OK.
 * @return ///< uint8_t StatusCode
 */
uint8_t nfc_requestA_or_wakeupA(nfc_rfid_t *nfc, uint8_t command, uint8_t *bufferATQA, uint8_t *bufferSize);

/**
 * @brief Transmits a REQuest command, Type A. 
 * Invites PICCs in state IDLE to go to READY and prepare for anticollision or selection. 7 bit frame.
 * 
 * @param nfc 
 * @param bufferATQA ///< The buffer to store the ATQA (Answer to request) in
 * @param bufferSize ///< Buffer size, at least two bytes. Also number of bytes returned if STATUS_OK.
 * @return uint8_t  StatusCode
 */
static inline uint8_t nfc_requestA(nfc_rfid_t *nfc, uint8_t *bufferATQA, uint8_t *bufferSize)
{
    return nfc_requestA_or_wakeupA(nfc, (uint8_t)PICC_CMD_REQA, bufferATQA, bufferSize);
}

/**
 * @brief Transmits a Wake-UP command, Type A. 
 * Invites PICCs in state IDLE and HALT to go to READY(*) and prepare for anticollision or selection. 7 bit frame.
 * 
 * @param nfc 
 * @param bufferATQA ///< The buffer to store the ATQA (Answer to request) in
 * @param bufferSize ///< Buffer size, at least two bytes. Also number of bytes returned if STATUS_OK.
 * @return uint8_t  StatusCode
 */
static inline uint8_t nfc_wakeupA(nfc_rfid_t *nfc, uint8_t *bufferATQA, uint8_t *bufferSize)
{
    return nfc_requestA_or_wakeupA(nfc, (uint8_t)PICC_CMD_WUPA, bufferATQA, bufferSize);
}

/**
 * @brief // Select slave
 * 
 * @param nfc 
 */
static inline void cs_select(nfc_rfid_t *nfc) {
    asm volatile("nop \n nop \n nop");
    gpio_put(nfc->pinout.cs, 0);  // Active low
    asm volatile("nop \n nop \n nop");
}

/**
 * @brief // Deselect slave
 * 
 * @param nfc 
 */
static inline void cs_deselect(nfc_rfid_t *nfc) {
    asm volatile("nop \n nop \n nop");
    gpio_put(nfc->pinout.cs, 1);
    asm volatile("nop \n nop \n nop");
}

/**
 * @brief Perform a write operation to the NFC.
 * 
 * @param nfc 
 * @param reg 
 * @param data 
 */
static inline void nfc_write(nfc_rfid_t *nfc, uint8_t reg, uint8_t data)
{
    uint8_t buf[2] = {reg, data};
    cs_select(nfc);
    spi_write_blocking(nfc->spi, buf, 2);
    cs_deselect(nfc);
}

/**
 * @brief Perform multiple write operations to the NFC.
 * 
 * @param nfc 
 * @param reg 
 * @param data 
 * @param len 
 */
static inline void nfc_write_mult(nfc_rfid_t *nfc, uint8_t reg, uint8_t *data, uint8_t len)
{
    uint8_t buf[64] = {reg};
    for (int i = 0; i < len; i++)
    {
        buf[i+1] = data[i];
    }
    cs_select(nfc);
    spi_write_blocking(nfc->spi, buf, len+1);
    cs_deselect(nfc);
}

/**
 * @brief Perform a read operation to the NFC.
 * 
 * @param nfc 
 * @param reg 
 * @return uint8_t 
 */
static inline uint8_t nfc_read(nfc_rfid_t *nfc, uint8_t reg)
{
    uint8_t data;
    reg = reg | READ_BIT;
    cs_select(nfc);
    spi_write_blocking(nfc->spi, &reg, 1);
    spi_read_blocking(nfc->spi, 0, &data, 1);
    cs_deselect(nfc);
    return data;
}

/**
 * @brief Perform multiple read operations to the NFC.
 * 
 * @param nfc 
 * @param reg 
 * @param data 
 * @param len 
 * @param rxAlign ///< Only bit positions rxAlign..7 in values[0] are updated. Default 0.
 */ 
static inline void nfc_read_mult(nfc_rfid_t *nfc, uint8_t reg, uint8_t *data, uint8_t len, uint8_t rxAlign)
{
    const uint8_t msg = reg | READ_BIT;
    cs_select(nfc);
    for (int i = 0; i < len; i++)
    {
        data[i] = nfc_read(nfc, msg);
    }
    cs_deselect(nfc);
    if (rxAlign)
    {
        uint8_t mask = (0xFF << rxAlign) & 0xFF;
        data[0] = (data[0] & ~mask) | (data[1] & mask);
    }
}

/**
 * @brief Clears the bits given in mask from register reg.
 * 
 * @param nfc 
 * @param reg 
 * @param mask ///< The bits to clear.
 */
static inline void nfc_clear_reg_bitmask(nfc_rfid_t *nfc, uint8_t reg, uint8_t mask)
{
    uint8_t value = nfc_read(nfc, reg);
    nfc_write(nfc, reg, (uint8_t)(value & (~mask)));
}

/**
 * @brief Sets the bits given in mask in register reg.
 * 
 * @param nfc 
 * @param reg 
 * @param mask 
 */
static inline void nfc_set_reg_bitmask(nfc_rfid_t *nfc, uint8_t reg, uint8_t mask)
{
    uint8_t value = nfc_read(nfc, reg);
    nfc_write(nfc, reg, (uint8_t)(value | mask));
}

/**
 * @brief Turn on the NFC antenna.
 * Turns the antenna on by enabling pins TX1 and TX2.
 * After a reset these pins are disabled.
 * 
 * @param nfc 
 */
static inline void nfc_antenna_on(nfc_rfid_t *nfc)
{
    uint8_t value = nfc_read(nfc, TxControlReg);
    if ((value & 0x03) != 0x03)
    {
        nfc_write(nfc, TxControlReg, value | 0x03);
    }
}

/**
 * @brief Performs a soft reset on the MFRC522 chip and waits for it to be ready again.
 * 
 * @param nfc 
 */
static inline void nfc_reset(nfc_rfid_t *nfc)
{
    nfc_write(nfc, CommandReg, PCD_SoftReset);
    uint8_t count = 0;
    do {
        sleep_ms(1);
    } while ((nfc_read(nfc, CommandReg) & (1<<4)) && ((++count) < 3));
}

/**
 * @brief Stops the encrypted data communication on the NFC.
 * 
 * @param nfc 
 */
static inline void nfc_stop_crypto1(nfc_rfid_t *nfc)
{
    nfc_clear_reg_bitmask(nfc, Status2Reg, 0x08);
}

/**
 * @brief Use the CRC coprocessor in the MFRC522 to calculate a CRC_A.
 * 
 * @param nfc 
 * @param data 
 * @param len 
 * @param result 
 * @return uint8_t STATUS_OK on success, STATUS_??? otherwise.
 */
uint8_t nfc_calculate_crc(nfc_rfid_t *nfc, uint8_t *data, uint8_t len, uint8_t *result);

/**
 * @brief Get the data of the product from the bufferRead.
 * 
 * @param nfc 
 * @return true if the ID is valid (range 1-7), false otherwise.
 */
bool nfc_get_data_tag(nfc_rfid_t *nfc);

/**
 * @brief Check the tag in the NFC.
 * 
 * @param nfc 
 */
bool nfc_check_tag(nfc_rfid_t *nfc);



#endif // __NFC_RFID_