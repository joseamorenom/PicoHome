#ifndef BLIND_H
#define BLIND_H

#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/sync.h"
#include "pico/flash.h"
#include "hardware/irq.h"
#include "stepper_motor.h"

#define FLASH_TARGET_OFFSET (PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE) ///< Flash-based address of the last sector
#define MAX_LOOPS 3 ///< Number of loops to open or close the blind

typedef struct {
    uint8_t position;
} blind_t;

extern stepper_motor_t gMotor;

void blind_init(blind_t *blind, uint8_t gpio_lsb);

void blind_open(blind_t *blind);
void blind_close(blind_t *blind);

void blind_read_flash(blind_t *blind);
void blind_write_flash(blind_t *blind);

/**
 * @brief Definition of the wrapper function for the flash_safe_execute function,
 * which is used to erase the last sector of the flash memory.
 * 
 */
static inline void blind_wrapper()
{
    flash_range_erase((PICO_FLASH_SIZE_BYTES - FLASH_SECTOR_SIZE), FLASH_SECTOR_SIZE);
}

#endif //BLING_H