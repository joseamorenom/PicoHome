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
#define KEYPAD_DBNC_TIME_MS 100
#define KEYPAD_PWM_SLICE 0

///< NFC pinout (SPI)
#define NFC_SCK 10
#define NFC_MOSI 11
#define NFC_MISO 12
#define NFC_CS 13
#define NFC_RST 14

///< Other pinout. All of them are digital GPIOs.
#define PIR_GPIO 16     ///< PIR sensor to detect movement
#define RELAY_GPIO 17   ///< Relay to control the door. 1 to open.
#define LOCK_GPIO 18    ///< Lock to know the state of the door: locked(1)/unlocked(0)
#define BUZZER_GPIO 27  ///< KY-006 buzzer

///< Other definitions
#define SEND_DOOR_TIME_MS 1000*10 ///< Time to send the brightness data to the broker
#define CHECK_TAG_TIME_MS 1000*1 ///< Time to check the presence of a tag

extern volatile flags_t gFlags;
extern mqtt_t gMqtt;
key_pad_t gKeyPad;
nfc_rfid_t gNFC;
pir_t gPIR;
door_t gDoor;

enum {INDOOR, OUTDOOR} gSystemState;

void app_init(void)
{

    gFlags.B = 0; ///< Clear all flags
    ///< Initialize the system modules
    gSystemState = INDOOR;
    app_init_mqtt();
    kp_init(&gKeyPad, KEYPAD_ROWS_LSB_GPIO, KEYPAD_COLS_LSB_GPIO, KEYPAD_DBNC_TIME_MS, true);
    nfc_init_as_spi(&gNFC, spi1, NFC_SCK, NFC_MOSI, NFC_MISO, NFC_CS, 22, NFC_RST);
    pir_init(&gPIR, PIR_GPIO, false);
    door_init(&gDoor, RELAY_GPIO, LOCK_GPIO);

    ///< Configure the alarm to send the brightness data to the broker
    struct repeating_timer timer;
    add_repeating_timer_ms(CHECK_TAG_TIME_MS, check_tag_timer_cb, NULL, &timer);

    ///< Set the PWM interrupt as PIT
    pwm_set_as_pit(KEYPAD_PWM_SLICE, KEYPAD_DBNC_TIME_MS, false); // 100ms for the button debouncer
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_handler);
    // gpio_add_raw_irq_handler_masked() //-> I like this function.

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
                subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_USER_ALARM);
            }
            if (gFlags.error_pub_mqtt) {
                printf("Error: pub_mqtt\n");
                gFlags.error_pub_mqtt = 0;
                publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_BRIGHTNESS, gMqtt.data.brightness, 2, 1);
            }
            ///< Check the flags to execute the corresponding functions
            if (gFlags.sys_keypad_switch){
                gFlags.sys_keypad_switch = 0;
                gKeyPad.cols = gpio_get_all() & (0x0000000f << gKeyPad.KEY.clsb); ///< Get columns gpio values
                kp_set_irq_rows(&gKeyPad); ///< Switch interrupt to columns
                gKeyPad.rows = gpio_get_all() & (0x0000000f << gKeyPad.KEY.rlsb); ///< Get rows gpio values
                pwm_set_enabled(gKeyPad.pwm_slice, true); ///< Set the debouncer alarm
            }
            if (gFlags.sys_check_tag){
                gFlags.sys_check_tag = 0;
                if (nfc_check_tag(&gNFC)){
                    printf("Tag detected\n");
                    door_open(&gDoor);
                }
            }
            
        }
        __wfi();
    }
}

void app_init_mqtt(void)
{
    ///< Setting the mqtt client credentials
    memset(&gMqtt.ci, 0, sizeof(gMqtt.ci));
    gMqtt.ci.client_id = MQTT_CLIENT_ID_ACCESS_CONTROL_UNIT;
    gMqtt.ci.keep_alive = 60;
    gMqtt.ci.client_user = MQTT_CLIENT_USER_ACCESS_CONTROL_UNIT;
    gMqtt.ci.client_pass = MQTT_CLIENT_PASS_ACCESS_CONTROL_UNIT;

    ///< Initialize the mqtt client
    while (!init_mqtt(&gMqtt.client, &gMqtt.ci, MQTT_SEVER_IP1)) {
        printf("It could not connect to the server. Trying again...\n");
        gFlags.error_init_mqtt = 1;
    }
    printf("MQTT client initialized\n");
    
    while (!subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_USER_ALARM)) {
        printf("It could not subscribe to the topic\n");
        gFlags.error_sub_mqtt = 1;
    }
    printf("Subscribed to the topic\n");
}

bool check_tag_timer_cb(struct repeating_timer *t)
{
    // Check for a tag entering
    if (gpio_get(LOCK_GPIO) && !gNFC.tag.is_present){ ///< Just check the tag if the door is locked
        if (nfc_is_new_tag(&gNFC)){
            gFlags.sys_check_tag = 1; ///< Activate the flag of the NFC interruption to read the card
        }
    }

    ///< Send the door state to the broker

    return false;
}

void gpio_cb(uint gpio, uint32_t events)
{
    switch (gSystemState)
    {
    case INDOOR:
        if (!gKeyPad.KEY.dbnc) {
            kp_set_irq_enabled(&gKeyPad, true, false); ///< Disable the columns interrupt
            gKeyPad.KEY.dbnc = 1; ///< Activate the debouncer
            gFlags.sys_keypad_switch = 1; ///< Activate the flag to switch the interrupt to columns
        }
        break;
    
    default:
        break;
    }
}

void pwm_handler(void)
{
    uint32_t rows;
    switch (pwm_get_irq_status_mask())
    {
    case 0x01UL: // PWM slice 0 is used for the button debouncer
        rows = gpio_get_all() & (0x0000000f << gKeyPad.KEY.rlsb); ///< Get rows gpio values
        if (!rows){ ///< If a key was not pressed, process the key and disable the debouncer
            gKeyPad.KEY.dbnc = 0;
            gFlags.sys_key_pressed = 1; ///< The key is processed once the debouncer is finished
            kp_set_irq_cols(&gKeyPad); ///< Switch interrupt to columns
            pwm_set_enabled(gKeyPad.pwm_slice, false); ///< Disable the debouncer PIT
        }
        pwm_clear_irq(0);         // Acknowledge slice 0 PWM IRQ
        break; 

    default:
        printf("Happend what should not happens on PWM IRQ\n");
        break;
    }
}

void pwm_set_as_pit(uint8_t slice, uint16_t milis, bool enable)
{
    assert(milis<=262);                  // PWM can manage interrupt periods greater than 262 milis
    float prescaler = (float)SYS_CLK_KHZ/500;
    assert(prescaler<256); // the integer part of the clock divider can be greater than 255 
                 // ||   counter frecuency    ||| Period in seconds taking into account de phase correct mode |||   
    uint32_t wrap = (1000*SYS_CLK_KHZ*milis/(prescaler*2*1000)); // 500000*milis/2000
    assert(wrap<((1UL<<17)-1));
    // Configuring the PWM
    pwm_config cfg =  pwm_get_default_config();
    pwm_config_set_phase_correct(&cfg, true);
    pwm_config_set_clkdiv(&cfg, prescaler);
    pwm_config_set_clkdiv_mode(&cfg, PWM_DIV_FREE_RUNNING);
    pwm_config_set_wrap(&cfg, wrap);
    pwm_set_irq_enabled(slice, true);
    irq_set_enabled(PWM_IRQ_WRAP, true);
    pwm_init(slice, &cfg, enable);
}

