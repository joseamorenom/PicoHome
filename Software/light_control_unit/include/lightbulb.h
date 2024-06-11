/**
 * \file        lightbulb.h
 * \brief
 * \details
 * 
 * 
 * \author      PicoHome
 * \version     0.0.1
 * \date        10/04/2024
 * \copyright   Unlicensed
 */

#ifndef __LIGHTBULB__
#define __LIGHTBULB__

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

/**
 * @brief Lightbulb structure
 * 
 */
typedef struct {
    uint8_t gpio;
    uint8_t slice;
    uint16_t duty_cycle;    ///< range 0-1000, where: 0 -> 0%, 1000 -> 100%
    uint16_t frequency;     ///< frequency in Hz
    uint32_t wrap;          ///< wrap value
}lightbulb_t;


/**
 * @brief Initialize the lightbulb
 * 
 * @param lightbulb 
 * @param gpio 
 * @param frequency 
 * @param duty_cycle range 0-1000, where: 0 -> 0%, 1000 -> 100%
 */
void lightbulb_init(lightbulb_t *lightbulb, uint8_t gpio, uint16_t frequency, uint16_t duty_cycle);

/**
 * @brief Set the duty cycle of the lightbulb
 * 
 * @param lightbulb 
 * @param duty_cycle range 0-1000, where: 0 -> 0%, 1000 -> 100%
 */
static inline void lightbulb_set_duty(lightbulb_t *lightbulb, uint16_t duty_cycle)
{
    lightbulb->duty_cycle = duty_cycle;
    pwm_set_chan_level(lightbulb->slice, PWM_CHAN_A, duty_cycle*lightbulb->wrap/1000);
}

/**
 * @brief Set the brightness of the lightbulb
 * 
 * @param lightbulb 
 * @param brightness range 0-100, where: 0 -> 0%, 100 -> 100%
 * @return true if the brightness is set, false otherwise
 */
static inline void lightbulb_set_brightness(lightbulb_t *lightbulb, uint16_t brightness)
{
    if (brightness > 100) return;
    lightbulb_set_duty(lightbulb, brightness*10);
}

/**
 * @brief Get the duty cycle of the lightbulb
 * 
 * @param lightbulb 
 * @return uint16_t 
 */
static inline uint16_t lightbulb_get_duty(lightbulb_t *lightbulb)
{
    return lightbulb->duty_cycle;
}

/**
 * @brief Set the frequency of the lightbulb
 * 
 * @param lightbulb 
 * @param frequency 
 */
static inline void lightbulb_set_frequency(lightbulb_t *lightbulb, uint16_t frequency)
{
    lightbulb->frequency = frequency;
    float prescaler = (float)SYS_CLK_KHZ/500;
    uint32_t wrap = (1000*SYS_CLK_KHZ/(prescaler*frequency));
    pwm_set_wrap(lightbulb->slice, wrap);
}

/**
 * @brief Get the frequency of the lightbulb
 * 
 * @param lightbulb 
 * @return uint16_t 
 */
static inline uint16_t lightbulb_get_frequency(lightbulb_t *lightbulb)
{
    return lightbulb->frequency;
}

/**
 * @brief Enable or disable the lightbulb
 * 
 * @param lightbulb 
 * @param enabled 
 */
static inline void lightbulb_enabled(lightbulb_t *lightbulb, bool enabled)
{
    pwm_set_enabled(lightbulb->slice, enabled);
}


#endif // __LIGHTBULB__