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
#define BUZZER_GPIO 26  ///< KY-006 buzzer

///< Other definitions
#define SEND_DOOR_TIME_S 5           ///< Send the door state to the broker every 5 seconds
#define CHECK_TAG_TIME_MS 1000*1     ///< Time to check the presence of a tag
#define SYS_PASS 1234                ///< System password
#define SYS_MAX_TRIES 3              ///< Maximum number of tries to enter the password
#define SYS_ALARM_WAIT_MS 30*1000    ///< Time to wait to turn on the PIR sensor after the alarm is activated
#define SYS_SEND_ALARM_TIME_S 10     ///< Send the alarm state to the broker every 5 seconds
#define SYS_WHATCHDOG_TIME_MS 10*1000   ///< Watchdog time
#define SYS_TIME_TOGGLING_LED_S 5    ///< Time to toggle the led

extern volatile flags_t gFlags;
extern mqtt_t gMqtt;
key_pad_t gKeyPad;
nfc_rfid_t gNFC;
pir_t gPIR;
door_t gDoor;
bool gLed = false;

enum {
    INDOOR, ///< Means: there is no alarm activated. It includes when the user goes out or comes in to the house
    ALARM,  ///< Means: the alarm is activated by the user with the key (0x0E -> *),
            ///< and the PIR sensor is waiting to detect a movement 
    INTRUDER ///< Means: incorrect password entered (or timeout) and the PIR sensor detected a movement
} gSystemState;

void app_init(void)
{
    // Enable the watchdog, requiring the watchdog to be updated every 100ms or the chip will reboot
    // second arg is pause on debug which means the watchdog will pause when stepping through code
    watchdog_enable(SYS_WHATCHDOG_TIME_MS, 1);

    gFlags.B = 0; ///< Clear all flags
    ///< Initialize the system modules
    gSystemState = INDOOR;
    kp_init(&gKeyPad, KEYPAD_ROWS_LSB_GPIO, KEYPAD_COLS_LSB_GPIO, KEYPAD_DBNC_TIME_MS, true);
    nfc_init_as_spi(&gNFC, spi1, NFC_SCK, NFC_MOSI, NFC_MISO, NFC_CS, 22, NFC_RST);
    pir_init(&gPIR, PIR_GPIO, false);
    door_init(&gDoor, RELAY_GPIO, LOCK_GPIO, SEND_DOOR_TIME_S);
    gpio_init(BUZZER_GPIO);
    gpio_set_dir(BUZZER_GPIO, GPIO_OUT);
    app_init_mqtt();

    ///< Configure the alarm to send the brightness data to the broker
    struct repeating_timer timer;
    add_repeating_timer_ms(CHECK_TAG_TIME_MS, check_tag_timer_cb, NULL, &timer);

    ///< Set the PWM interrupt as PIT
    pwm_set_as_pit(KEYPAD_PWM_SLICE, KEYPAD_DBNC_TIME_MS, false); // 100ms for the button debouncer
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_handler);
    // gpio_add_raw_irq_handler_masked() //-> I like this function, but it is not implemented yet.

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
                subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_USER_ALARM);
            }

            ///< Check the flags to execute the corresponding functions
            if (gFlags.broker_connected) { ///< The broker is connected
                gFlags.broker_connected = 0;
                led_on();
                add_alarm_in_ms(SYS_TIME_TOGGLING_LED_S*1000, led_toggle_timer_cb, NULL, false);
            }
            if (gFlags.broker_alarm){ ///< Ther user turn off the alarm
                gFlags.broker_alarm = 0;
                gSystemState = INDOOR;
                gFlags.sys_send_alarm = 0;
                pir_set_irq_enabled(&gPIR, false);
                nfc_init_as_spi(&gNFC, spi1, NFC_SCK, NFC_MOSI, NFC_MISO, NFC_CS, 22, NFC_RST);
                gpio_put(BUZZER_GPIO, 0);
                printf("Change to indoor - Broker: %d\n", atoi(gMqtt.data.alarm));
                publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_ALARM, "0", 2, 1);
            }
            if (gFlags.sys_keypad_switch){
                gFlags.sys_keypad_switch = 0;
                gKeyPad.cols = gpio_get_all() & (0x0000000f << gKeyPad.KEY.clsb); ///< Get columns gpio values
                kp_set_irq_rows(&gKeyPad); ///< Switch interrupt to columns
                gKeyPad.rows = gpio_get_all() & (0x0000000f << gKeyPad.KEY.rlsb); ///< Get rows gpio values
                pwm_set_enabled(gKeyPad.pwm_slice, true); ///< Set the debouncer alarm
            }
            if (gFlags.sys_check_tag){
                gFlags.sys_check_tag = 0;
                if (nfc_check_is_valid_tag(&gNFC)){
                    printf("Valid tag detected\n");
                    door_open(&gDoor);
                }
                led_on();
                char str[14]; ///< 1 bytes of tag access + 1 byte of comma + 8 bytes of the tag UID + 3 bytes colon + 1 byte of '\0'
                snprintf(str, sizeof(str), "%d,%02x:%02x:%02x:%02x", gNFC.tag.uid_reg_access, 
                        (uint8_t)(gNFC.tag.uid>>24), (uint8_t)(gNFC.tag.uid>>16), (uint8_t)(gNFC.tag.uid>>8), (uint8_t)(gNFC.tag.uid));
                publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_NFC, str, 2, true);
            }
            if (gFlags.sys_send_door){
                gFlags.sys_send_door = 0;
                publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_DOOR, door_is_open(&gDoor)?"0":"1", 2, true);
            }
            if (gFlags.sys_key_pressed){
                gFlags.sys_key_pressed = 0;
                kp_capture(&gKeyPad);
                printf("Key pressed: %d\n", gKeyPad.KEY.dkey);
                app_process_key();
            }
            if (gFlags.sys_send_alarm){
                gFlags.sys_send_alarm = 0;
                publish(gMqtt.client, NULL, MQTT_TOPIC_PUB_ALARM, "1", 2, true);
                gSystemState = INTRUDER;
                gpio_put(BUZZER_GPIO, 1);
                printf("Alarm sent to the broker\n");
            }
        }
        __wfi();
    }
}

void app_process_key()
{
    static uint8_t key_cnt = 0;
    static uint16_t key_value = 0;
    static uint8_t cnt_tries = 0;
    switch (gSystemState)
    {
    case INDOOR: ///< Press the key (0x0E) to change to alarm state
        if (gKeyPad.KEY.dkey == 0x0E){ ///< Turn on the alarm state
            add_alarm_in_ms(SYS_ALARM_WAIT_MS, alarm_timer_cb, NULL, false);
            printf("Turn on alarm mode\n");
        }
        break;
    case ALARM: ///< Enter the password to change to indoor
        if (checkNumber(gKeyPad.KEY.dkey)){ ///< Check if the key is a number
            key_value = key_value*10 + gKeyPad.KEY.dkey;
            key_cnt++;
            printf("Key value: %d\n", key_value);
            if (key_cnt == 4){ ///< Check if the password is complete
                if (key_value == SYS_PASS){
                    cnt_tries = 0;
                    gPIR.pass_correct = true;
                    gSystemState = INDOOR;
                    pir_set_irq_enabled(&gPIR, false);
                    printf("Change to indoor\n");
                }
                else if (++cnt_tries == SYS_MAX_TRIES){
                    cnt_tries = 0;
                    gFlags.sys_send_alarm = 1; ///< Activate the flag to send the alarm to the broker
                    printf("Invalid password\n");
                }
                key_cnt = 0;
                key_value = 0;
            }
        }
        else { ///< The key is not a number
            cnt_tries++;
            key_cnt = 0;
            key_value = 0;
            printf("Invalid key\n");
        }
        break;
    
    default:
        break;
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
        return;
    }
    printf("MQTT client initialized\n");
    
    ///< Subscribe to the topics: alarm send by the user
    while (!subscribe_topic(&gMqtt.client, MQTT_TOPIC_SUB_USER_ALARM)) {
        printf("It could not subscribe to the topic\n");
        gFlags.error_sub_mqtt = 1;
        return;
    }
    printf("Subscribed to the topic\n");
}

bool check_tag_timer_cb(struct repeating_timer *t)
{
    ///< Update the watchdog
    watchdog_update();

    // Check for a tag entering
    if (!door_is_open(&gDoor)){ ///< Just check the tag if the door is locked
        if (nfc_is_new_tag(&gNFC)){
            gFlags.sys_check_tag = 1; ///< Activate the flag of the NFC interruption to read the card
        }
    }

    ///< Send the door state to the broker
    gDoor.cnt_send_door++;
    if (gDoor.cnt_send_door >= gDoor.time_send_door){
        gDoor.cnt_send_door = 0;
        gFlags.sys_send_door = 1; ///< Activate the flag to send the door state to the broker
    }

    ///< Send the alarm to the broker every second
    if (gSystemState == INTRUDER && !gFlags.sys_send_alarm) {
        gFlags.sys_send_alarm = 1; ///< Activate the flag to send the alarm to the broker
    }

    ///< Led system
    if (gSystemState ==  ALARM){
        led_toggle();
    }
    else if (gSystemState == INTRUDER){
        led_on();
    }
    else {
        led_off();
    }

    return true;
}

int64_t alarm_timer_cb(alarm_id_t id, void *data)
{
    switch (gSystemState)
    {
    case INDOOR:
        pir_set_irq_enabled(&gPIR, true);
        gPIR.pass_correct = false;
        gSystemState = ALARM;
        printf("30 seconds. PIR turn on\n");
        break;

    case ALARM:
        if (!gPIR.pass_correct){
            gFlags.sys_send_alarm = 1; ///< Activate the flag to send the alarm to the broker
            printf("30s passed. Alarm sent\n");
        }
        break;
    
    default:
        break;
    }
    return 0;
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

void gpio_cb(uint gpio, uint32_t events)
{
    switch (gSystemState)
    {
    case INDOOR: ///< The user press the key (0x0E) to change to alarm state
        if (!gKeyPad.KEY.dbnc && gpio != gPIR.gpio) {
            kp_set_irq_enabled(&gKeyPad, true, false); ///< Disable the columns interrupt
            gKeyPad.KEY.dbnc = 1; ///< Activate the debouncer
            gFlags.sys_keypad_switch = 1; ///< Activate the flag to switch the interrupt to columns
        }
        break;
    case ALARM: ///< The system is in alarm state and a gpio interruption is detected: pir or keypad
        if (gpio == gPIR.gpio){
            pir_set_irq_enabled(&gPIR, false); ///< Just one interruption is allowed
            add_alarm_in_ms(SYS_ALARM_WAIT_MS, alarm_timer_cb, NULL, false);
            printf("Set timer for PIR sensor\n");
        }
        else if (!gKeyPad.KEY.dbnc){
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
