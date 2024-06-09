cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

set(CMAKE_C_STANDARD 11)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


target_include_directories(mqtt_client PUBLIC ../include/)

target_link_libraries(mqtt_client
                      pico_stdlib
                      pico_cyw43_arch_lwip_threadsafe_background
                      pico_cyw43_arch_none
                      pico_lwip_mqtt)


pico_enable_stdio_uart(mqtt_client 0)
pico_enable_stdio_usb(mqtt_client 1)
