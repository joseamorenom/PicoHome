/**
 * \file        functs.c
 * \brief
 * \details
 * 
 * 
 * \author      PicoHome
 * \version     0.0.1
 * \date        10/04/2024
 * \copyright   Unlicensed
 */
#include "app.h"

#define KY018_GPIO 26           ///< GPIO of the KY-018 sensor

#define LIGHTBULB_GPIO 18       ///< GPIO of the lightbulb
#define LIGTHBULB_FREQUENCY 60  ///< 60Hz
#define LIGHTBULB_DUTY_CYCLE 50 ///< 50%

#define SEND_DATA_TIME_MS 1000*5  ///< Send data to the broker every 2 seconds

extern volatile flags_t gFlags;
extern mqtt_t gMqtt;
sen_ky018_t gKy018;
lightbulb_t gLightbulb;
volatile char Hola[4];

void app_init(void)
{

    gFlags.B = 0; ///< Clear all flags
    ///< Initialize the system modules
    app_init_mqtt();
    ky018_init(&gKy018, KY018_GPIO);
    lightbulb_init(&gLightbulb, LIGHTBULB_GPIO, LIGTHBULB_FREQUENCY, LIGHTBULB_DUTY_CYCLE);

    printf("System initialized\n");
    ///< Configure the alarm to send the brightness data to the broker
    struct repeating_timer timer;
    add_repeating_timer_ms(SEND_DATA_TIME_MS, send_brightness_timer_cb, NULL, &timer);
    printf("Timer configured\n");

    app_main();

}

void app_main(void)
{
    printf("Main loop\n");
    while (true){
        while (gFlags.B) {
            ///< Error handling
            if (gFlags.error_init_mqtt) {
                printf("Error: init_mqtt\n");
                gFlags.error_init_mqtt = 0;
                app_init_mqtt();
                ///< Turn on any warning led
            }
            if (gFlags.error_sub_mqtt) {
                printf("Error: sub_mqtt\n");
                gFlags.error_sub_mqtt = 0;
                subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_BRIGHTNESS);
            }
            if (gFlags.error_pub_mqtt) {
                printf("Error: pub_mqtt\n");
                gFlags.error_pub_mqtt = 0;
                publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_BRIGHTNESS, gMqtt.data.brightness, 2, 1);
            }
            ///< Check the flags to execute the corresponding functions
            if (gFlags.broker_brightness) {
                printf("Broker: %s\n", gMqtt.data.brightness);
                gFlags.broker_brightness = 0;
                lightbulb_set_brightness(&gLightbulb, atoi(gMqtt.data.brightness));
            }
            if (gFlags.sys_send_brightness) {
                gFlags.sys_send_brightness = 0;
                uint8_t brightness = ky018_get_luxes(&gKy018); ///< Get the brightness value
                if (brightness <= 100){ ///< Check if the value is valid
                    snprintf(gMqtt.data.brightness, sizeof(gMqtt.data.brightness), "%d", brightness);
                    publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_BRIGHTNESS, gMqtt.data.brightness, 2, 1);
                }
                printf("KY018: %d\n", brightness);
            }
        }
        __wfi();
    }
}

void app_init_mqtt(void)
{
    ///< Setting the mqtt client credentials
    memset(&gMqtt.ci, 0, sizeof(gMqtt.ci));
    gMqtt.ci.client_id = MQTT_CLIENT_ID_LIGHT_CONTROL_UNIT;
    gMqtt.ci.keep_alive = 60;
    gMqtt.ci.client_user = MQTT_CLIENT_USER_LIGHT_CONTROL_UNIT;
    gMqtt.ci.client_pass = MQTT_CLIENT_PASS_LIGHT_CONTROL_UNIT;

    ///< Initialize the mqtt client
    while (!init_mqtt(&gMqtt.client, &gMqtt.ci, MQTT_SEVER_IP1)) {
        printf("It could not connect to the server. Trying again...\n");
        gFlags.error_init_mqtt = 1;
    }
    printf("MQTT client initialized\n");
    
    while (!subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_BRIGHTNESS)) {
        printf("It could not subscribe to the topic\n");
        gFlags.error_sub_mqtt = 1;
    }
    printf("Subscribed to the topic\n");
}

bool send_brightness_timer_cb(struct repeating_timer *t)
{
    gFlags.sys_send_brightness = 1;

    return true;
}
