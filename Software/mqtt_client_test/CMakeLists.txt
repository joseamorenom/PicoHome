
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

target_include_directories(test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

pico_enable_stdio_uart(test 0)
pico_enable_stdio_usb(test 1)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(test)

# Memory usage
SET(GCC_EXE_LINKER_FLAGS    "-Wl,--print-memory-usage")

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_EXE_LINKER_FLAGS}")
