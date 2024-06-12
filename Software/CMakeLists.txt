cmake_minimum_required(VERSION 3.12)
set(PICO_BOARD pico_w)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(PicoHome C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        )
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

# Hardware-specific examples in subdirectories:
add_subdirectory(mqtt_client_test)
add_subdirectory(mqtt_client)
add_subdirectory(access_control_unit) # PicoHome Node
add_subdirectory(blinds_control_unit) # PicoHome Node
add_subdirectory(light_control_unit) # PicoHome Node 

#add_subdirectory(polling_microPython)
