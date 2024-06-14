/**
 * \file        pir.h
 * \brief
 * \details
 * \author      PicoHome
 * \version     0.0.2
 * \date        03/05/2024
 * \copyright   Unlicensed
 */

#ifndef __PIR__
#define __PIR__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "hardware/gpio.h"
#include "app.h"

/**
 * \typedef pir_t
 * \brief Data strcuture to manage a PIR sensor
 */
typedef struct{
    uint8_t gpio;   ///< GPIO connected to the PIR sensor
    uint8_t en;     ///< Enable PIR processing
    bool pass_correct;   ///< Flag that indicates that a movement was detected
}pir_t;                       ///< All PIR related information

/**
 * @brief This method initializes the PIR data structure
 * 
 * @param pir pointer to PIR data structure
 * @param gpio GPIO connected to the PIR sensor
 * @param en True if PIR start enabled
 */
void pir_init(pir_t *pir, uint8_t gpio, bool en);

/**
 * @brief Enable or disable the gpio interrupt for the PIR sensor.
 * 
 * @param pir 
 * @param en 
 */
static inline void pir_set_irq_enabled(pir_t *pir, bool en)
{
    pir->en = en;
    gpio_set_irq_enabled(pir->gpio, GPIO_IRQ_EDGE_RISE, en);
}

#endif // __PIR__

