/**
 * \file        blind.c
 * \brief
 * \details
 * 
 * 
 * \author      PicoHome
 * \version     0.0.1
 * \date        10/06/2024
 * \copyright   Unlicensed
 */

#include "blind.h"

void blind_init(blind_t *blind, stepper_motor_t *motor, uint8_t gpio_lsb, uint8_t motor_mode)
{
    blind->position = 0;

    stepper_motor_init(motor, 1.8, motor_mode, gpio_lsb);
    //blind_read_flash(blind); // Read the last position from the flash
}

void blind_open(blind_t *blind, stepper_motor_t *motor)
{
    if (blind->position == 100)
    {
        stepper_motor_loop(motor, MAX_LOOPS); // Positive direction
        blind->position = 0;
    }
    // blind_write_flash(blind);
}

void blind_close(blind_t *blind, stepper_motor_t *motor)
{
    if (blind->position == 0)
    {
        stepper_motor_loop(motor, -MAX_LOOPS); // Negative direction
        blind->position = 100;
    }
    // blind_write_flash(blind);
}

// void blind_read_flash(blind_t *blind)
// {
//     // Compute the memory-mapped address, remembering to include the offset for RAM
//     uint32_t addr = XIP_BASE +  FLASH_TARGET_OFFSET;
//     uint32_t *ptr = (uint32_t *)addr; ///< Place an int pointer at our memory-mapped address

//     blind->position = *ptr;
// }

// void blind_write_flash(blind_t *blind)
// {
//     // Erase the last sector of the flash
//     flash_safe_execute(blind_wrapper, NULL, 500);

//     uint32_t ints = save_and_disable_interrupts();
//     flash_range_program(FLASH_TARGET_OFFSET, (uint8_t *)&blind->position, FLASH_PAGE_SIZE);
//     restore_interrupts (ints);
// }
