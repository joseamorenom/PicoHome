/**
 * \file        app.c
 * \brief
 * \details
 * 
 * 
 * \author      PicoHome
 * \version     0.0.1
 * \date        10/06/2024
 * \copyright   Unlicensed
 */
#include "app.h"

#define SEND_DATA_TIME_MS 1000*5  ///< Send data to the broker every 5 seconds
#define SYS_WHATCHDOG_TIME_MS 10*1000 ///< Watchdog time in ms
#define GPIO_MOTOR_LSB 18 ///< GPIO pin of the LSB of the stepper motor
#define MOTOR_MODE 8 ///< Step mode of the stepper motor (Half step)
#define SYS_TIME_TOGGLING_LED_S 5 ///< Time to toggle the led


/// @brief Global and extern variables (dont change this two: gFlags and gMqtt)
extern volatile flags_t gFlags;
extern mqtt_t gMqtt;
stepper_motor_t gMotor; ///< Global variable of the stepper motor
blind_t gBlind; ///< Global variable of the blind
bool gLed = false; ///< Global variable of the led


void app_init(void)
{
    // Enable the watchdog, requiring the watchdog to be updated every 100ms or the chip will reboot
    // second arg is pause on debug which means the watchdog will pause when stepping through code
    watchdog_enable(SYS_WHATCHDOG_TIME_MS, 1);

    gFlags.B = 0; ///< Clear all flags
    ///< Initialize the system modules
    app_init_mqtt();
    blind_init(&gBlind, &gMotor, GPIO_MOTOR_LSB, MOTOR_MODE);

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
            if (gFlags.error_init_mqtt) { ///< Error initializing the mqtt client
                printf("Error: init_mqtt\n");
                gFlags.error_init_mqtt = 0;
                app_init_mqtt();
                ///< Turn on any warning led
            }
            if (gFlags.error_sub_mqtt) { ///< Error subscribing to the topic
                printf("Error: sub_mqtt\n");
                gFlags.error_sub_mqtt = 0;
                subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_USER_BLINDS);
            }
            if (gFlags.error_pub_mqtt) { ///< Error sending data to the broker
                printf("Error: pub_mqtt\n");
                gFlags.error_pub_mqtt = 0;
                publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_BLINDS, gMqtt.data.brightness, 2, 1);
            }
            ///< Check the flags to execute the corresponding functions
            if (gFlags.broker_connected) { ///< The broker is connected
                gFlags.broker_connected = 0;
                led_on();
            }
            if (gFlags.broker_blinds) { ///< Blinds data received from the broker
                printf("Broker blinds\n");
                gFlags.broker_blinds = 0;
                if (strcmp(gMqtt.data.blinds, "1") == 0) {
                    blind_open(&gBlind, &gMotor);
                } else if (strcmp(gMqtt.data.blinds, "0") == 0) {
                    blind_close(&gBlind, &gMotor);
                }
            }
            if (gFlags.sys_send_blinds) { ///< Send the blinds data to the broker
                printf("Send blinds\n");
                gFlags.sys_send_blinds = 0;
                char blinds[3]; 
                uint8_t position = gBlind.position;
                if (position == 0) {
                    sprintf(blinds, "1");
                } else if (position == 100) {
                    sprintf(blinds, "0");
                }
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

    led_off();

    return true;
}

int64_t led_toggle_timer_cb(alarm_id_t id, void *data)
{
    static uint8_t cnt = 0;
    led_toggle();
    if (++cnt == SYS_TIME_TOGGLING_LED_S){
        cnt = 0;
        led_off();
    }
    return 0;
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
