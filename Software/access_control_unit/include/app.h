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
#include "pico/time.h"
#include "pico/flash.h"
#include "hardware/sync.h"
#include "hardware/pwm.h"
#include "hardware/watchdog.h"

#include "mqtt_client.h"
#include "keypad.h"
#include "nfc_rfid.h"
#include "pir.h"
#include "door.h"

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
 * @brief This function initializes a PWM signal as a periodic interrupt timer (PIT).
 * Each slice will generate interruptions at a period of milis miliseconds.
 * Due to each slice share clock counter (period), events with diferents periods 
 * must not be generated in the same slice, i.e, they must not share channel.
 * 
 * @param slice 
 * @param milis Period of the PWM interruption in miliseconds
 * @param enable 
 */
void pwm_set_as_pit(uint8_t slice, uint16_t milis, bool enable);

/**
 * @brief Process the key pressed by the user, tanking into account the current state of the system.
 * 
 * @param slice 
 * @param pin 
 */
void app_process_key();

// ------------------------------------------------------------------------
// -------------------- CALLBACK AND HANDLER FUNCTIONS --------------------
// ------------------------------------------------------------------------

/**
 * @brief Callback function of the timer to check the presence of a tag
 * 
 * @param t 
 */
bool check_tag_timer_cb(struct repeating_timer *t);

/**
 * @brief Callback function to process the alarm mode.
 * It is used when the user turn on the alarm mode (press * key).
 * Also it is used when someone is detected by the PIR sensor.
 * 
 * @param id 
 * @param data 
 * @return int64_t 
 */
int64_t alarm_timer_cb(alarm_id_t id, void *data);

/**
 * @brief This function is the callback function for the GPIO interruptions.
 * 
 * @param num 
 * @param mask 
 */
void gpio_cb(uint gpio, uint32_t events);

/**
 * @brief This function is the handler for the PWM interruptions.
 * 
 */
void pwm_handler(void);

// -------------------------------------------------------------
// ---------------------- Check functions ----------------------
// -------------------------------------------------------------

static inline bool checkNumber(uint8_t number){
    return (number >= 0 && number <= 9);
}

static inline bool checkLetter(uint8_t letter){
    return (letter >= 0x0A && letter <= 0x0D);
}

#endif // __APP_H__
