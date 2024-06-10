/**
 * \file        functs.c
 * \brief
 * \details
 * 
 * 
 * \author      PicoHome
 * \version     0.0.1
 * \date        10/04/2024
 * \copyright   Unlicensed
 */
#include "app.h"

volatile flags_t gFlags;
volatile mqtt_t gMqtt;

void app_init(void)
{

    gFlags.B = 0; ///< Clear all flags
}
