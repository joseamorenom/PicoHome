
cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

set(CMAKE_C_STANDARD 11)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(MQTT_Test)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(test
	main.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(test 
    pico_stdlib
	hardware_sync
    mqtt_client_lib
	)

add_library(mqtt_client_lib STATIC ../mqtt_client/mqtt_client.c)

target_include_directories(mqtt_client_lib PUBLIC ../mqtt_client/include/)

target_link_libraries(mqtt_client_lib
                    pico_stdlib
                    pico_cyw43_arch_lwip_threadsafe_background
                    pico_cyw43_arch_none
                    pico_lwip_mqtt)


pico_enable_stdio_uart(test 0)
pico_enable_stdio_usb(test 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(test)

# Memory usage
SET(GCC_EXE_LINKER_FLAGS    "-Wl,--print-memory-usage")

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_EXE_LINKER_FLAGS}")
