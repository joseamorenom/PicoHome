/**
 * \file        pir.c
 * \brief
 * \details
 * 
 * 
 * \author      PicoHome
 * \version     0.0.1
 * \date        10/06/2024
 * \copyright   Unlicensed
 */

#include "door.h"

void door_init(door_t *door, uint8_t gpio_in_relay, uint8_t gpio_lock, uint8_t time_send_door)
{
    door->time_send_door = time_send_door;
    door->gpio_in_relay = gpio_in_relay;
    door->gpio_lock = gpio_lock;
    gpio_init(door->gpio_in_relay);
    gpio_init(door->gpio_lock);
    gpio_set_dir(door->gpio_in_relay, GPIO_OUT);
    gpio_set_dir(door->gpio_lock, GPIO_IN);
    gpio_put(door->gpio_in_relay, 0);
    gpio_pull_down(door->gpio_lock);
}

void door_open(door_t *door)
{
    if (door_is_open(door)){ ////< The door is already open{
        return;
    }
    gpio_put(door->gpio_in_relay, 1);
    add_alarm_in_ms(DOOR_OPEN_TIME_MS, door_close_cb, door, false);
}

