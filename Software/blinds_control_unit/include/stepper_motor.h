/**
 * @file stepper_motor.h
 * @author PicoHome
 * @brief This library allows you to control a stepper motor with a Raspberry Pi Pico used in the blinds control unit project, we use
 * interrupts generate for the timer to control the stepper motor
 * @version 0.1
 * @date 2024-06-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

//#include "blind.h"

#define MOTOR_SPEED_US 1000
#define ONE_LOOP_4_STEPS 2048
#define ONE_LOOP_8_STEPS 4096


typedef struct
{
    uint32_t step;
    uint32_t step_target;
    float angle;
    uint8_t step_mode;
    uint8_t step_dir;
    uint8_t gpio_lsb;
    uint8_t *steps;

    uint8_t timer_irq;

}stepper_motor_t;

extern stepper_motor_t gMotor;
//extern blind_t gBlind;

/**
 * @brief Initialize the stepper motor
 * 
 * @param motor ///< Struct of the stepper motor
 * @param step_angle ///< Add here the step angle of the stepper motor
 * @param step_mode ///< Add here the step mode of the stepper motor
 * @param gpio_lsb  ///< Add here the GPIO pin of the LSB 
 */
void stepper_motor_init(stepper_motor_t *motor, double step_angle, uint8_t step_mode, uint8_t gpio_lsb);

/**
 * @brief Move the stepper motor
 * 
 * @param motor ///< Struct of the stepper motor
 * @param steps ///< Add here the number of steps to move
 */
void stepper_motor_move(stepper_motor_t *motor, int32_t steps);

/**
 * @brief Set num of loops to move the stepper motor
 * 
 * @param motor 
 * @param loops 
 */
void stepper_motor_loop(stepper_motor_t *motor, int16_t loops);

/**
 * @brief Set the alarm to move the stepper motor
 * 
 * @param motor ///< Struct of the stepper motor
 */
void stepper_motor_set_alarm(stepper_motor_t *motor);

/**
 * @brief Timer handler for the stepper motor, this function is called every time the timer is triggered for give a step to the motor
 * 
 */
void stepper_motor_timer_handler(void);
#endif // STEPPER_MOTOR_H