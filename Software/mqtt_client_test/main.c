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

    if(publish(client, NULL, MQTT_TOPIC_PUB_BRIGHTNESS, "100", 2, 1)) {
        printf("Correctly published\n");
    }


    while(true) {
        __wfi();
    }

    return 0;
}