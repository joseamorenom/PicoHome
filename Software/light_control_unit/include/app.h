/**
 * \file        app.h
 * \brief       API of the Light Control Unit
 * \details
 * \author      Picohome
 * \version     0.0.1
 * \date        09/06/2024
 * \copyright   Unlicensed
 */

#ifndef __APP_H__
#define __APP_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "mqtt_client.h"

/**
 * @brief Flags to control the application
 * 
 */
typedef union {
    uint8_t B; ///< Byte access
    struct {
        uint8_t send_light  : 1; ///< Flag to send light data to the broker
        uint8_t set_light   : 1; ///< Flag to set the light value
        uint8_t             : 6;
    };
} flags_t; 

/**
 * @brief Initialize the application: Light Control Unit
 * 
 */
void app_init(void);



#endif // __APP_H__
