/**
 * \file        lightbulb.c
 * \brief       
 * \details
 * 
 * 
 * \author      PicoHome
 * \version     0.0.1
 * \date        10/04/2024
 * \copyright   Unlicensed
 */

#include "lightbulb.h"

void lightbulb_init(lightbulb_t *lightbulb, uint8_t gpio, uint16_t frequency, uint16_t duty_cycle)
{
    lightbulb->gpio = gpio;
    lightbulb->slice = pwm_gpio_to_slice_num(gpio);
    lightbulb->frequency = frequency;
    lightbulb->duty_cycle = duty_cycle;
    
    float prescaler = (float)SYS_CLK_KHZ/500;
    uint32_t wrap = (1000*SYS_CLK_KHZ/(prescaler*frequency));
    lightbulb->wrap = wrap;

    gpio_set_function(lightbulb->gpio, GPIO_FUNC_PWM);
    pwm_set_clkdiv(lightbulb->slice, prescaler);
    pwm_set_clkdiv_mode(lightbulb->slice, PWM_DIV_FREE_RUNNING);
    pwm_set_wrap(lightbulb->slice, wrap);
    pwm_set_chan_level(lightbulb->slice, PWM_CHAN_A, duty_cycle*wrap/1000);
    pwm_set_enabled(lightbulb->slice, true);
}

