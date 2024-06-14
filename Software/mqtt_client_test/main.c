/**
 * \file        test.c
 * \brief       File to test the mqtt client
 * \details     It is a simple program to test the mqtt client connection to the broker.
 * It is used to test the connection to the broker, the subscription to a topic and the publication of a message.
 * Change the values of the macros to test the connection to the broker, including the IP address of the broker, 
 * the client ID, the user and the password, where there last three are the credentials of a MQTT Device.
 * 
 * 
 * \author      PicoHome
 * \version     0.0.1
 * \date        10/04/2024
 * \copyright   Unlicensed
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "mqtt_client.h"
#include "hardware/sync.h"

int main(void)
{
    stdio_init_all();

    sleep_ms(3000);

    struct mqtt_connect_client_info_t ci;
    mqtt_client_t *client;
    memset(&ci, 0, sizeof(ci));
    ci.client_id = MQTT_CLIENT_ID_LIGHT_CONTROL_UNIT;
    ci.keep_alive = 60;
    ci.client_user = MQTT_CLIENT_USER_LIGHT_CONTROL_UNIT;
    ci.client_pass = MQTT_CLIENT_PASS_LIGHT_CONTROL_UNIT;
    
    while (!init_mqtt(&client, &ci, MQTT_SEVER_IP1)) {
        printf("It could not connect to the server. Trying again...\n");
    }
    
    while (!subscribe_topic(&client, MQTT_TOPIC_SUB_USER_BRIGHTNESS)) {
        printf("It could not subscribe to the topic\n");
    }
    printf("Subscribed to the topic\n");
    // char data[2] = "7\0";

    if(publish(client, NULL, MQTT_TOPIC_PUB_BRIGHTNESS, "Hello world from pico_w", 2, 1)) {
        printf("Correctly published\n");
    }


    while(true) {
        __wfi();
    }

    return 0;
}