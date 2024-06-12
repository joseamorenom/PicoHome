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

///< Keypad pinout
#define KEYPAD_ROWS_LSB_GPIO 2
#define KEYPAD_COLS_LSB_GPIO 6

///< NFC pinout (SPI)
#define NFC_SCK 10
#define NFC_MOSI 11
#define NFC_MISO 12
#define NFC_CS 13
#define NFC_RST 14

///< Other pinout. All of them are digital GPIOs.
#define PIR_GPIO 16     ///< PIR sensor to detect movement
#define RELAY_GPIO 17   ///< Relay to control the door: open/close
#define LOCK_GPIO 18    ///< Lock to the state of the door: locked/unlocked
#define BUZZER_GPIO 27  ///< KY-006 buzzer

extern volatile flags_t gFlags;
extern mqtt_t gMqtt;
key_pad_t gKeypad;
nfc_rfid_t gNFC;

void app_init(void)
{

    gFlags.B = 0; ///< Clear all flags
    ///< Initialize the system modules
    app_init_mqtt();

    ///< Configure the alarm to send the brightness data to the broker
    // struct repeating_timer timer;
    // add_repeating_timer_ms(SEND_DATA_TIME_MS, send_brightness_timer_cb, NULL, &timer);

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
                subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_USER_BRIGHTNESS);
            }
            if (gFlags.error_pub_mqtt) {
                printf("Error: pub_mqtt\n");
                gFlags.error_pub_mqtt = 0;
                publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_BRIGHTNESS, gMqtt.data.brightness, 2, 1);
            }
            ///< Check the flags to execute the corresponding functions
            
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
    
    while (!subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_USER_BRIGHTNESS)) {
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
