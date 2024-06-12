/**
 * \file        pir.c
 * \brief
 * \details
 * 
 * 
 * \author      PicoHome
 * \version     0.0.1
 * \date        10/04/2024
 * \copyright   Unlicensed
 */

#include "pir.h"

void pir_init(pir_t *pir, uint8_t gpio, bool en)
{
    pir->gpio = gpio;
    pir->en = en;
    pir->pir = 0;
    gpio_init_mask(1 << pir->gpio);
    gpio_set_dir(pir->gpio, GPIO_IN);
    gpio_pull_down(pir->gpio);
    gpio_set_irq_enabled_with_callback(pir->gpio, GPIO_IRQ_EDGE_RISE, en, gpio_cb);
}