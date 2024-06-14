/**
 * \file        main.h
 * \brief       Application interface of the Light Control Unit
 * 
 * \details     The light control unit is a device that controls the brightness of a light bulb
 * using a PWM signal. The light control unit is connected to a broker using MQTT and send the light value
 * to the broker. 
 * The light control unit can receive messages from the broker to set the light value.
 * 
 * \author      Picohome
 * \version     0.0.1
 * \date        09/06/2024
 * \copyright   Unlicensed
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "app.h"

int main(void)
{
    sleep_ms(3000);
    stdio_init_all();

    ///< Initialize and run the application
    app_init();
    
    return 0;
}

