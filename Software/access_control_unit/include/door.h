/**
 * \file        door.h
 * \brief
 * \details
 * \author      PicoHome
 * \version     0.0.2
 * \date        03/05/2024
 * \copyright   Unlicensed
 */

#ifndef __DOOR__
#define __DOOR__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/time.h"
#include "hardware/gpio.h"

#define DOOR_OPEN_TIME_MS 1000 ///< Time to keep the door open

/**
 * \typedef door_t
 * \brief Data strcuture to manage a door
 */
typedef struct{
    uint8_t gpio_in_relay;   ///< GPIO connected to the relay that opens the door
    uint8_t gpio_lock;   ///<  Indicates that the door was opened
}door_t; ///< All door related information


/**
 * @brief This method initializes the door data structure
 * 
 * @param door pointer to door data structure
 * @param gpio_in_relay GPIO connected to the door
 * @param gpio_lock GPIO connected to the relay that opens the door
 */
void door_init(door_t *door, uint8_t gpio_in_relay, uint8_t gpio_lock);

/**
 * @brief This method opens the door
 * 
 * @param door pointer to door data structure
 */
void door_open(door_t *door);

/**
 * @brief This method closes the door
 * 
 * @param door pointer to door data structure
 */
static inline void door_close(door_t *door)
{
    gpio_put(door->gpio_in_relay, 0);
}

/**
 * @brief Get the state of the door: 1 if it is open, 0 if it is closed
 * 
 * @param door 
 */
static inline bool door_is_open(door_t *door){
    return gpio_get(door->gpio_lock);
}

#endif // __DOOR__