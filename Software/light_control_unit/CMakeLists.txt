cmake_minimum_required(VERSION 3.15)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(light_Control_Unit)
set(PICO_BOARD pico_w)
set(CMAKE_C_STANDARD 11)

pico_sdk_init()

add_executable(light_control_unit 
    ./src/main.c)

target_link_libraries(light_control_unit
    pico_stdlib
    app
    lightbulb_lib
    )

target_include_directories(light_control_unit PUBLIC ../include)

add_subdirectory(src)
add_subdirectory(include)

pico_add_extra_outputs(light_control_unit)

pico_enable_stdio_usb(light_control_unit 1)
pico_enable_stdio_uart(light_control_unit 0)

# Memory usage
SET(GCC_EXE_LINKER_FLAGS    "-Wl,--print-memory-usage")

SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_EXE_LINKER_FLAGS}")