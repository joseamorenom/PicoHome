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
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define R_REF 10000.0
#define V_REF 3.3
#define ADC_RANGE (1 << 12)
#define ADC_CONVERT (V_REF / (ADC_RANGE - 1))

#define R0 10.0 * 1000  // Resistance at I0 (e.g., 10 kΩ at 100 lux)
#define E0 100.0        // Reference illuminance (100 lux)
#define GAMMA 0.7       // Photoresistor constant

/**
 * @brief KY-018 structure
 * 
 */
typedef struct {
    uint8_t adc_num;
    uint8_t gpio;
    uint8_t luxes;
}sen_ky018_t;

/**
 * @brief Initialize the KY-018 sensor
 * 
 * @param ky018 KY-018 sensor
 */
void ky018_init(sen_ky018_t *ky018, uint8_t gpio);

/**
 * @brief Read the value of the KY-018 sensor
 * 
 * @param ky018 KY-018 sensor
 */
uint8_t ky018_get_luxes(sen_ky018_t *ky018);



#endif // __SEN_KY018__