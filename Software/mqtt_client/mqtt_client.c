/**
 * @file mqtt_client
 * @brief Implementation of the mqtt client based on lwip library (mqtt application layer)
 * @version 0.1
 * @date 2024-06-07
 * @authors PicoHome
*/

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "mqtt_client.h"
#include <string.h>

uint8_t inpub_id;

static bool init_wifi(void)
{
    if(cyw43_arch_init_with_country(CYW43_COUNTRY_COLOMBIA))
        return false;

    cyw43_arch_enable_sta_mode(); ///< Power on the wifi module

    if(cyw43_arch_wifi_connect_timeout_ms(SSID, PASS, CYW43_AUTH_WPA2_MIXED_PSK, 10000))
        return false;

    return true;
}

bool init_mqtt(mqtt_client_t **client, struct mqtt_connect_client_info_t *ci, const uint8_t *ip_server)
{   
    ip_addr_t broker_ip;
    err_t err;


    while(!init_wifi()) {
        printf("ERROR: wifi module not initialized. Trying again...\n");
    }

    *client = mqtt_client_new();
    
    if(!*client) {
        printf("ERROR: mqtt client not initialized\n");
        return false;
    }
    
    ipaddr_aton(ip_server, &broker_ip); ///< Convert the ip address to a binary format
    err = mqtt_client_connect(*client, &broker_ip, MQTT_PORT, mqtt_connection_cb, 0, ci);
    if (err != ERR_OK) {
        printf("ERROR: mqtt client not connected\n");
        return false;
    }
    return true;
    
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    if(status == MQTT_CONNECT_ACCEPTED) {
        printf("Conection acepted\n");
        mqtt_set_inpub_callback(client, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, arg);
    
    } else {
        printf("Conection rejected. Status: %d\n", status);
    }
}

static void mqtt_incoming_publish_cb(void *arg, const char *topic, uint32_t tot_len)
{
    printf("Incoming publish at topic %s with total length %u\n", topic, (unsigned int)tot_len);

    if(strcmp(topic, MQTT_TOPIC_SUB_BRIGHTNESS) == 0) {
        printf("Received data from BRGTHNESS subscription\n");
        inpub_id = 0;                       /*<! Si el tema es el tema por defecto el id de la publicación es 0*/
    }
    else if(strcmp(topic, MQTT_TOPIC_PUB_BRIGHTNESS) == 0) {
        printf("Received data from BRGTHNESS publication\n");
        inpub_id = 1;                       /*<! Si lo que se recibe es una respuesta luego de publicar */
    }
}

static void mqtt_incoming_data_cb(void *arg, const uint8_t *data, uint16_t len, uint8_t flags)
{
    printf("Incoming publish payload with length %d, flags %u\n", len, (unsigned int)flags);
    uint8_t in_data[len];
    memcpy(in_data, data, len);
    if(flags && MQTT_DATA_FLAG_LAST) { ///< The last data was received

        switch (inpub_id) 
        {
        case 0:
            printf("BRIGHTNESS: %s\n", in_data);
            break;
        case 1:
            printf("RESPONSE: %s\n", in_data);
            break;
        default:
            break;
        }
    }
}

bool subscribe_topic(mqtt_client_t **client, const uint8_t *subtopic)
{
    err_t err;
    err = mqtt_subscribe(*client, subtopic, 1, NULL, NULL);

    return err == ERR_OK;
}

bool publish(mqtt_client_t *client, void *arg, const uint8_t *topic, 
            const uint8_t *payload, const uint8_t qos, const uint8_t retain)
{
    err_t err;
    err = mqtt_publish(client, topic, payload, strlen(payload), qos, retain, mqtt_pub_request_cb, arg);
    
    return err == ERR_OK;
}

static void mqtt_pub_request_cb(void *arg, err_t result)
{

    if(result != ERR_OK) {
        printf("ERROR: publication failed - result: %d\n", result);
    }
    else {
        printf("Publication success - result: %d\n", result);
    }
}