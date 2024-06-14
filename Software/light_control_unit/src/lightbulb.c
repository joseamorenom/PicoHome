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
    lightbulb->channel = pwm_gpio_to_channel(gpio);
    lightbulb->frequency = frequency;
    lightbulb->duty_cycle = duty_cycle;

    float prescaler = (float)SYS_CLK_KHZ/500;
    uint16_t wrap = (1000*SYS_CLK_KHZ)/(prescaler*frequency);
    lightbulb->wrap = wrap;
    pwm_gpio_to_channel(gpio);

    gpio_set_function(lightbulb->gpio, GPIO_FUNC_PWM);
    pwm_set_clkdiv(lightbulb->slice, prescaler);
    pwm_set_wrap(lightbulb->slice, wrap);
    pwm_set_chan_level(lightbulb->slice, lightbulb->channel, (duty_cycle*wrap)/100);
    pwm_set_enabled(lightbulb->slice, true);
}

