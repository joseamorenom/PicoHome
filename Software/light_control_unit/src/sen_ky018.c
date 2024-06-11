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

void ky018_init(sen_ky018_t *ky018, uint8_t gpio)
{
    assert(gpio >= 26 && gpio <= 29);
    ky018->gpio = gpio;
    ky018->adc_num = gpio - 26;
    adc_init();
    adc_gpio_init(ky018->gpio);
    adc_select_input(ky018->adc_num);
}

uint8_t ky018_get_luxes(sen_ky018_t *ky018)
{
    uint16_t adc_value = adc_read();

    ///< Convert to voltage (0-Vcc)
    float voltage = adc_value * (ADC_CONVERT);
    
    ///< Calculate the resistance of the LDR
    float resistance = 0;
    if (voltage != 0) {
        resistance = (V_REF * R_REF) / (V_REF - voltage);
    }
    
    ///< Calculate luxes using the potential relationship
    ky018->luxes = 0;
    if (resistance != 0) {
        ky018->luxes = E0 * pow((R0 / resistance), (1 / GAMMA));
    }

    if (ky018->luxes > 100) {
        ky018->luxes = 19;
    }
    
    return ky018->luxes;
}
