/**
 * \file        stepper_motor.h
 * \brief
 * \details
 * \author      PicoHome
 * \version     0.0.2
 * \date        08/06/2024
 * \copyright   Unlicensed
 */

#include "stepper_motor.h"

/**
 * @brief Constants for the step mode
 * 
 */
const uint8_t STEPS_4_VALUES[] = {0x01, 0x02, 0x04, 0x08};
const uint8_t STEPS_8_VALUES[] = {0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08, 0x09};

void stepper_motor_init(stepper_motor_t * motor, double step_angle, uint8_t step_mode, uint8_t gpio_lsb)
{
    // Initialize the stepper motor parameters
    motor->step = 0;
    motor->gpio_lsb = gpio_lsb;
    motor->step_mode = step_mode;
    motor->timer_irq = TIMER_IRQ_1;

    //Asign the step mode to the stepper motor of the MACROS
    switch (step_mode)
    {
        case 4:
            motor->steps = (uint8_t *)STEPS_4_VALUES;
            break;

        case 8:
            motor->steps = (uint8_t *)STEPS_8_VALUES;
            break;
        default:
            break;
    }

    // Initialize the GPIO pins
    gpio_init_mask(0x0F << gpio_lsb);
    gpio_set_dir_masked(0x0F << gpio_lsb, 0X0F << gpio_lsb);
    gpio_put_masked(0x0F << gpio_lsb, 0x00 << gpio_lsb);
}

void stepper_motor_move(stepper_motor_t *motor, int32_t steps)
{
    motor->step_dir = steps > 0 ? 1 : 0;
    motor->step = 0;
    motor->step_target = abs(steps);

    stepper_motor_set_alarm(motor);
}

void stepper_motor_loop(stepper_motor_t *motor, int16_t loops)
{
    if (motor->step_mode == 4)
    {
        stepper_motor_move(motor, ONE_LOOP_4_STEPS * loops);
    }
    else if (motor->step_mode == 8)
    {
        stepper_motor_move(motor, ONE_LOOP_8_STEPS * loops);
    }
}

void stepper_motor_set_alarm(stepper_motor_t *motor)
{
    // Interrupt acknowledge
    hw_clear_bits(&timer_hw->intr, 1u << motor->timer_irq);

    // Setting the IRQ handler
    irq_set_exclusive_handler(motor->timer_irq, stepper_motor_timer_handler);
    irq_set_enabled(motor->timer_irq, true);
    hw_set_bits(&timer_hw->inte, 1u << motor->timer_irq); ///< Enable alarm0 interrupt
    timer_hw->alarm[motor->timer_irq] = (uint32_t)(time_us_64() + MOTOR_SPEED_US); ///< Set alarm to 500us
}

void stepper_motor_timer_handler(void)
{
    hw_clear_bits(&timer_hw->intr, 1u << gMotor.timer_irq);

    if (gMotor.step < gMotor.step_target)
    {
        switch (gMotor.step_dir)
        {
            case 0:
                //change the direction of the motor
                gpio_put_masked(0x0F << gMotor.gpio_lsb, gMotor.steps[(gMotor.step_mode - 1) - (gMotor.step % gMotor.step_mode)] << gMotor.gpio_lsb);
                break;
            case 1:
                gpio_put_masked(0x0F << gMotor.gpio_lsb, gMotor.steps[gMotor.step % gMotor.step_mode] << gMotor.gpio_lsb);
                break;
            default:
                break;
        }
        gMotor.step++;
        // Setting the IRQ handler
        irq_set_exclusive_handler(gMotor.timer_irq, stepper_motor_timer_handler);
        irq_set_enabled(gMotor.timer_irq, true);
        hw_set_bits(&timer_hw->inte, 1u << gMotor.timer_irq); ///< Enable alarm0 interrupt
        timer_hw->alarm[gMotor.timer_irq] = (uint32_t)(time_us_64() + MOTOR_SPEED_US); ///< Set alarm to 500us
    }
    else
    {
        // Stop the motor
        gpio_put_masked(0x0F << gMotor.gpio_lsb, 0x00 << gMotor.gpio_lsb);
        gMotor.step = 0;
        gMotor.step_target = 0;
    }
    //printf("Step %d\n", motor.step % motor.step_mode);
    //printf("Bits %02X\n", motor.steps[motor.step % motor.step_mode]);

    //gBlind.position = gMotor.step/gMotor.step_target;

}
