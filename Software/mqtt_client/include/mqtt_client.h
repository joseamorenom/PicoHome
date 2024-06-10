#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "pico/cyw43_arch.h"
#include "lwip/apps/mqtt.h"
#include "lwip/dns.h"

#define SSID "Hinestroza"
#define PASS "6B004524"

#define MQTT_CHANNEL_ID "2571668"
#define MQTT_TOPIC_SUB_BRIGHTNESS "channels/2571668/subscribe/fields/field1"
#define MQTT_TOPIC_PUB_BRIGHTNESS "channels/2571668/publish/fields/field1"
#define MQTT_TOPIC_SUB_NFC "channels/2571668/subscribe/fields/field2"
#define MQTT_TOPIC_PUB_NFC "channels/2571668/publish/fields/field2"

#define MQTT_CLIENT_ID "LwU7EwAXNRkxKxQZISAJPBw"
#define MQTT_CLIENT_USER "LwU7EwAXNRkxKxQZISAJPBw"
#define MQTT_CLIENT_PASS "Ztf0mwq4qtQ1HRXHxsJ5YWAZ"
#define MQTT_SEVER_IP1 "18.207.44.162"
#define MQTT_SEVER_IP2 "54.81.146.55"

/**
 * @brief Struct to store the mqtt client and the client info
 * 
 */
typedef struct {
    struct mqtt_connect_client_info_t ci;
    mqtt_client_t *client;
}mqtt_t;

/**
 * @brief Function to initialize the wifi module
 * 
 * @return true if the wifi module was initialized correctly
 * @return false if an error occurred
 */
static bool init_wifi(void);

/**
 * @brief Function to initialize the mqtt client
 * 
 * @param client mqtt client
 * @param ci mqtt client info
 * @param ip_server ip server
 * @return true if the mqtt client was initialized correctly
 * @return false if an error occurred
 */
bool init_mqtt(mqtt_client_t **client, struct mqtt_connect_client_info_t *ci, const char *ip_server);

/**
 * @brief Callback function to when a connection occurs, defines the callback functions
 * for when a message is received
 * 
 * @param client mqtt client
 * @param arg argument to the callback function
 * @param status connection status
 */
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);

/**
 * @brief Function to handle the incoming publish message
 * 
 * @param arg argument to the callback function
 * @param topic topic where the message was published
 * @param tot_len total length of the message
 */
static void mqtt_incoming_publish_cb(void *arg, const char *topic, uint32_t tot_len);

/**
 * @brief Function to handle the incoming data
 * 
 * @param arg argument to the callback function
 * @param data data received
 * @param len length of the data
 * @param flags flags of the data
 */
static void mqtt_incoming_data_cb(void *arg, const uint8_t *data, uint16_t len, uint8_t flags);

/**
 * @brief Function to handle the publish request
 * 
 * @param arg argument to the callback function
 * @param err result of the publish request
 */
static void mqtt_pub_request_cb(void *arg, err_t result);

/**
 * @brief Function to subscribe to a topic
 * 
 * @param client mqtt client
 * @param topic topic to subscribe
 * @return true if the topic was subscribed correctly
 * @return false if an error occurred
 */
bool subscribe_topic(mqtt_client_t **client, const char *subtopic);

/**
 * @brief Function to publish a message
 * 
 * @param client mqtt client
 * @param arg argument to the callback function
 * @param topic topic to publish the message
 * @param payload message to publish
 * @param qos quality of service, 0,1,2 (0 by default)
 * @param retain indicates if the message should be saved in the broker
 * @return true if the message was published correctly
 * @return false if an error occurred
 */
bool publish(mqtt_client_t *client, void *arg, const char *topic, 
            const char *payload, const uint8_t qos, const uint8_t retain);

#endif // MQTT_CLIENT_H