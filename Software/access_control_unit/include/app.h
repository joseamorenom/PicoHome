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
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "pico/time.h"

#include "mqtt_client.h"
#include "keypad_irq.h"
#include "nfc_rfid.h"


/**
 * @brief Initialize the application: Light Control Unit
 * 
 */
void app_init(void);

/**
 * @brief Main function of the application
 * This has to be called in the main function of the application.
 * Contains the main loop of the application and checks the flags to execute the corresponding functions.
 * 
 */
void app_main(void);

/**
 * @brief Initialize the MQTT client
 * 
 */
void app_init_mqtt(void);

/**
 * @brief Callback function of the timer to send the brightness data to the broker
 * 
 * @param t 
 */
bool send_brightness_timer_cb(struct repeating_timer *t);

/**
 * @brief This function is the callback function for the GPIO interruptions.
 * 
 * @param num 
 * @param mask 
 */
void gpio_cb(uint gpio, uint32_t events);

#endif // __APP_H__
