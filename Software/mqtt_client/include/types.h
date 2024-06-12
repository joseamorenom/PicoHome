/**
 * \file        types.h
 * \brief
 * \details
 * 
 * 
 * \author      PicoHome
 * \version     0.0.1
 * \date        10/04/2024
 * \copyright   Unlicensed
 */

#ifndef __TYPES_H__
#define __TYPES_H__

#include <stdint.h>
#include <stdbool.h>
// #include "mqtt_client.h"
#include "lwip/apps/mqtt.h"

/**
 * @brief Flags to control the application
 * 
 */
typedef union {
    uint16_t B; ///< Byte access
    struct {
        /// @brief Flags to control broker events
        uint8_t broker_brightness   : 1; ///< Flag that indicates that light data was received from the broker
        uint8_t broker_blinds       : 1; ///< Flag that indicates that blinds data was received from the broker

        /// @brief Flags to control the system events
        uint8_t sys_send_brightness  : 1; ///< Flag to send light data to the broker
        uint8_t sys_send_blinds      : 1; ///< Flag to send blinds data to the broker
        uint8_t sys_check_tag        : 1; ///< Flag to check the presence of a tag
        uint8_t sys_keypad_switch    : 1; ///< Flag to switch the keypad irq from cols to rows
        uint8_t sys_key_pressed      : 1; ///< Flag to process the key pressed
    
        /// @brief Flags to control the errors
        uint8_t error_init_mqtt     : 1; ///< Mqtt client initilization has an error
        uint8_t error_pub_mqtt      : 1; ///< Error sending data to the broker
        uint8_t error_sub_mqtt      : 1; ///< Error subscribing to a topic

        uint16_t                    : 9;
    };

}flags_t; 

/**
 * @brief Struct to store the mqtt client and the client info
 * 
 */
typedef struct {
    struct mqtt_connect_client_info_t ci;
    mqtt_client_t *client;
    struct {
        char brightness[4]; ///< Light data
        char blinds[4];     ///< Blinds data
    }data;
}mqtt_t;


extern volatile flags_t gFlags; ///< Global flags to control the application
extern mqtt_t gMqtt; ///< Global mqtt client


#endif // __TYPES_H__