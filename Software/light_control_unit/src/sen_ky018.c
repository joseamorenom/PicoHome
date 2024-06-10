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


#include "sen_ky018.h"
#include "hardware/adc.h"

void sen_ky018_init(sen_ky018_t *ky018, uint8_t gpio)
{
    assert(gpio >= 26 && gpio <= 29);
    ky018->gpio = gpio;
    ky018->adc_num = gpio - 26;
    adc_init();
    adc_gpio_init(ky018->gpio);
    adc_select_input(ky018->adc_num);
}

uint8_t sen_ky018_read(sen_ky018_t *ky018)
{
    ky018->value = adc_read();
    ky018->value = (ky018->value * ADC_CONVERT)*CONVERT_VOLT_TO_LUX; ///< Convert digital value to lux: 0 - 100
    return ky018->value;
}
