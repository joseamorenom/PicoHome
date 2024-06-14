/**
 * \file        main.h
 * \brief       Application interface of the Blinds Control Unit
 * 
 * \details     The Blinds Control Unit is a device that controls the blinds of the house.
 * It has a motor to open and close the blinds, where ther user can control the blinds from a Web interface.
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

