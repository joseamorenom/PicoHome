/**
 * \file        door.h
 * \brief
 * \details
 * \author      PicoHome
 * \version     0.0.2
 * \date        08/06/2024
 * \copyright   Unlicensed
 */

#ifndef __DOOR__
#define __DOOR__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "pico/time.h"
#include "hardware/gpio.h"

#define DOOR_OPEN_TIME_MS 1000 ///< Time to keep the door open

/**
 * \typedef door_t
 * \brief Data strcuture to manage a door
 */
typedef struct{
    uint8_t gpio_in_relay;  ///< GPIO connected to the relay that opens the door
    uint8_t gpio_lock;      ///< Indicates if the door is open or closed: 0 if it is open, 1 if it is closed
    uint8_t cnt_send_door;  ///< Counter to send the door state to the broker
    uint8_t time_send_door; ///< Time to send the door state to the broker. Asumes an alarm that triggers every second
}door_t; ///< All door related information


/**
 * @brief This method initializes the door data structure
 * 
 * @param door pointer to door data structure
 * @param gpio_in_relay GPIO connected to the door
 * @param gpio_lock GPIO connected to the relay that opens the door
 * @param time_send_door (seg) Time to send the door state to the broker 
 */
void door_init(door_t *door, uint8_t gpio_in_relay, uint8_t gpio_lock, uint8_t time_send_door);

/**
 * @brief This method opens the door by activating the relay and setting an alarm to close it after a certain time.
 * 
 * @param door pointer to door data structure
 */
void door_open(door_t *door);

/**
 * @brief This method closes the door
 * 
 * @param id 
 * @param door 
 * @return int64_t 
 */
static inline int64_t door_close_cb(alarm_id_t id, void *door)
{
    door_t *d = (door_t *)door;
    gpio_put(d->gpio_in_relay, 0);
    return 0;
}

/**
 * @brief Get the state of the door: 1 if it is open, 0 if it is closed
 * 
 * @param door 
 */
static inline bool door_is_open(door_t *door){
    return !gpio_get(door->gpio_lock);
}

#endif // __DOOR__