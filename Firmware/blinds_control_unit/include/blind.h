/**
 * \file        blind.h
 * \brief
 * \details
 * \author      PicoHome
 * \version     0.0.2
 * \date        08/06/2024
 * \copyright   Unlicensed
 */

#ifndef __BLIND_
#define __BLIND_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
// #include "pico/flash.h"
#include "pico/time.h"
// #include "hardware/flash.h"
// #include "hardware/exception.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "hardware/irq.h"
#include "stepper_motor.h"

#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE) ///< Flash-based address of the last sector
#define MAX_LOOPS 2 ///< Number of loops to open or close the blind

typedef struct {
    uint8_t position; ///< Position of the blind (0-100)
} blind_t;

/**
 * @brief Initialize the blind structure with the motor and the GPIOs
 * 
 * @param blind Structure to be initialized
 * @param motor Structure of the motor
 * @param gpio_lsb Number of the LSB GPIO pin of the motor
 * @param motor_mode Mode operation of the motor (FULL_STEP or HALF_STEP)
 */
void blind_init(blind_t *blind, stepper_motor_t *motor, uint8_t gpio_lsb, uint8_t motor_mode);

/**
 * @brief Open the blind by rotating the motor
 * 
 * @param blind Structure of the blind
 * @param motor Structure of the motor
 */
void blind_open(blind_t *blind, stepper_motor_t *motor);

/**
 * @brief Close the blind by rotating the motor
 * 
 * @param blind Structure of the blind
 * @param motor Structure of the motor
 */
void blind_close(blind_t *blind, stepper_motor_t *motor);

// void blind_read_flash(blind_t *blind);
// void blind_write_flash(blind_t *blind);

// /**
//  * @brief Definition of the wrapper function for the flash_safe_execute function,
//  * which is used to erase the last sector of the flash memory.
//  * 
//  */
// static inline void blind_wrapper()
// {
//     flash_range_erase((PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE), FLASH_SECTOR_SIZE);
// }

#endif //__BLIND_