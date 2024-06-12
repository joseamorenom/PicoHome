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

#define SEND_DATA_TIME_MS 1000*5  ///< Send data to the broker every 5 seconds
#define SYS_WHATCHDOG_TIME_MS 10*1000 ///< Watchdog time in ms


/// @brief Global and extern variables (dont change this two: gFlags and gMqtt)
extern volatile flags_t gFlags;
extern mqtt_t gMqtt;

void app_init(void)
{
    // Enable the watchdog, requiring the watchdog to be updated every 100ms or the chip will reboot
    // second arg is pause on debug which means the watchdog will pause when stepping through code
    watchdog_enable(SYS_WHATCHDOG_TIME_MS, 1);

    gFlags.B = 0; ///< Clear all flags
    ///< Initialize the system modules
    app_init_mqtt();

    ///< Configure the alarm to send the brightness data to the broker
    struct repeating_timer timer;
    add_repeating_timer_ms(SEND_DATA_TIME_MS, send_blinds_timer_cb, NULL, &timer);

    watchdog_update();

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
                subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_USER_BLINDS);
            }
            if (gFlags.error_pub_mqtt) {
                printf("Error: pub_mqtt\n");
                gFlags.error_pub_mqtt = 0;
                publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_BLINDS, gMqtt.data.brightness, 2, 1);
            }
            ///< Check the flags to execute the corresponding functions
            if (gFlags.broker_blinds) { ///< Blinds data received from the broker
                printf("Broker blinds\n");
                gFlags.broker_blinds = 0;
                ///< ADD HERE THE FUNCTION TO CONTROL THE BLINDS
            }
            if (gFlags.sys_send_blinds) { ///< Send the blinds data to the broker
                printf("Send blinds\n");
                gFlags.sys_send_blinds = 0;
                char blinds[2]; ///< ADD HERE THE FUNCTION TO GET THE BLINDS DATA
                publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_BLINDS, blinds, 2, 1);
            }
            
        }
        __wfi();
    }
}

void app_init_mqtt(void)
{
    ///< Setting the mqtt client credentials
    memset(&gMqtt.ci, 0, sizeof(gMqtt.ci));
    gMqtt.ci.client_id = MQTT_CLIENT_ID_BLINDS_CONTROL_UNIT;
    gMqtt.ci.keep_alive = 60;
    gMqtt.ci.client_user = MQTT_CLIENT_USER_BLINDS_CONTROL_UNIT;
    gMqtt.ci.client_pass = MQTT_CLIENT_PASS_BLINDS_CONTROL_UNIT;

    ///< Initialize the mqtt client
    while (!init_mqtt(&gMqtt.client, &gMqtt.ci, MQTT_SEVER_IP1)) {
        printf("It could not connect to the server. Trying again...\n");
        gFlags.error_init_mqtt = 1;
        return;
    }
    printf("MQTT client initialized\n");
    
    while (!subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_USER_BLINDS)) {
        printf("It could not subscribe to the topic\n");
        gFlags.error_sub_mqtt = 1;
        return;
    }
    printf("Subscribed to the topic\n");
}

bool send_blinds_timer_cb(struct repeating_timer *t)
{

    ///< Update the watchdog
    watchdog_update();

    gFlags.sys_send_blinds = 1;

    return true;
}