/**
 * \file        main.h
 * \brief       Application interface of the Access Control Unit
 * 
 * \details     The Access Control Unit is a device that controls the access to the house. 
 * It has a PIR sensor to detect movement, a door that can be opened by a relay, 
 * a buzzer to alert of an unauthorized access, a NFC RFID reader to allow access to the house with a valid card, 
 * a keypad to enter a password and disable the alarm, and a LED to indicate the status of the system.
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

