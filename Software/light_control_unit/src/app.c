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

#define LIGHTBULB_GPIO 19       ///< GPIO of the lightbulb
#define LIGTHBULB_FREQUENCY_HZ 1000  ///< 60Hz
#define LIGHTBULB_DUTY_CYCLE 50 ///< 50%

#define SEND_DATA_TIME_MS 1000*5  ///< Send data to the broker every 5 seconds
#define SYS_WHATCHDOG_TIME_MS 15*1000   ///< Watchdog time

extern volatile flags_t gFlags;
extern mqtt_t gMqtt;
sen_ky018_t gKy018;
lightbulb_t gLightbulb;
bool gLed = false;

void app_init(void)
{
    // Enable the watchdog, requiring the watchdog to be updated every 100ms or the chip will reboot
    // second arg is pause on debug which means the watchdog will pause when stepping through code
    watchdog_enable(SYS_WHATCHDOG_TIME_MS, 1);

    gFlags.B = 0; ///< Clear all flags
    ///< Initialize the system modules
    app_init_mqtt();
    ky018_init(&gKy018, KY018_GPIO);
    lightbulb_init(&gLightbulb, LIGHTBULB_GPIO, LIGTHBULB_FREQUENCY_HZ, LIGHTBULB_DUTY_CYCLE);

    ///< Configure the alarm to send the brightness data to the broker
    struct repeating_timer timer;
    add_repeating_timer_ms(SEND_DATA_TIME_MS, send_brightness_timer_cb, NULL, &timer);

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
                subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_USER_BRIGHTNESS);
            }
            if (gFlags.error_pub_mqtt) {
                printf("Error: pub_mqtt\n");
                gFlags.error_pub_mqtt = 0;
                publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_BRIGHTNESS, gMqtt.data.brightness, 2, 1);
            }
            ///< Check the flags to execute the corresponding functions
            if (gFlags.broker_connected) { ///< The broker is connected
                gFlags.broker_connected = 0;
                led_on();
            }
            if (gFlags.broker_brightness) {
                gFlags.broker_brightness = 0;
                uint8_t brightness = atoi(gMqtt.data.brightness);
                if (brightness <= 0) {
                    lightbulb_enabled(&gLightbulb, false);
                } else {
                    lightbulb_set_brightness(&gLightbulb, brightness);
                }

                printf("String: %s\n", gMqtt.data.brightness);
                printf("Broker: %d\n", atoi(gMqtt.data.brightness));
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
        return;
    }
    printf("MQTT client initialized\n");
    
    while (!subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_USER_BRIGHTNESS)) {
        printf("It could not subscribe to the topic\n");
        gFlags.error_sub_mqtt = 1;
        return;
    }
    printf("Subscribed to the topic\n");
}

bool send_brightness_timer_cb(struct repeating_timer *t)
{

    ///< Update the watchdog
    watchdog_update();

    gFlags.sys_send_brightness = 1;

    return true;
}

void led_toggle()
{
    gLed = !gLed;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, gLed);
}

void led_on()
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
}


void led_off()
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
}
