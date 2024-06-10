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
        uint8_t brightness  : 1; ///< Flag that indicates that light data was received from the broker
        uint8_t             : 7;
    }broker;
    struct {
        uint8_t send_light  : 1; ///< Flag to send light data to the broker
        uint8_t             : 7;
    }sys;
} flags_t; 

/**
 * @brief Struct to store the mqtt client and the client info
 * 
 */
typedef struct {
    struct mqtt_connect_client_info_t ci;
    mqtt_client_t *client;
    struct {
        char brightness[4]; ///< Light data
    }data;
}mqtt_t;


extern volatile flags_t gFlags; ///< Global flags to control the application
extern volatile mqtt_t gMqtt; ///< Global mqtt client


#endif // __TYPES_H__