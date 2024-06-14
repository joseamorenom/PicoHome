/**
 * \file        app.h
 * \brief       API of the Blinds Control Unit
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
#include "pico/cyw43_arch.h"
#include "hardware/sync.h"
#include "hardware/watchdog.h"
#include "pico/time.h"

#include "mqtt_client.h"
#include "stepper_motor.h"
#include "blind.h"

/**
 * @brief Initialize the application: Blind Control Unit
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
bool send_blinds_timer_cb(struct repeating_timer *t);

/**
 * @brief Callback function to toggle de led when system is connected to the broker.
 * 
 * @param id 
 * @param data 
 * @return int64_t 
 */
int64_t led_toggle_timer_cb(alarm_id_t id, void *data);

/**
 * @brief Toggle the led of the pico_w board
 * 
 */
void led_toggle();

/**
 * @brief Turn on the led of the pico_w board
 * 
 */
void led_on();

/**
 * @brief Turn off the led of the pico_w board
 * 
 */
void led_off();

#endif // __APP_H__
