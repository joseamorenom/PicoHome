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

#ifndef __SEN_KY018__
#define __SEN_KY018__

#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define ADC_VREF 3.3
#define ADC_RANGE (1 << 12)
#define ADC_CONVERT (ADC_VREF / (ADC_RANGE - 1))
#define CONVERT_VOLT_TO_LUX 100 / ADC_VREF

/**
 * @brief KY-018 structure
 * 
 */
typedef struct {
    uint8_t adc_num;
    uint8_t gpio;
    uint8_t value;
}sen_ky018_t;

/**
 * @brief Initialize the KY-018 sensor
 * 
 * @param ky018 KY-018 sensor
 */
void sen_ky018_init(sen_ky018_t *ky018, uint8_t gpio);

/**
 * @brief Read the value of the KY-018 sensor
 * 
 * @param ky018 KY-018 sensor
 */
uint8_t sen_ky018_read(sen_ky018_t *ky018);



#endif // __SEN_KY018__