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
#include "sen_ky018.h"
#include "lightbulb.h"


/**
 * @brief Initialize the application: Light Control Unit
 * 
 */
void app_init(void);



#endif // __APP_H__
