add_library(mqtt_client_lib STATIC mqtt_client.c)

target_include_directories(mqtt_client_lib PUBLIC ./include)

target_link_libraries(mqtt_client_lib
                    pico_stdlib
                    pico_cyw43_arch_lwip_threadsafe_background
                    pico_cyw43_arch_none
                    pico_lwip_mqtt)