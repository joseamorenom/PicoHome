# APP library
add_library(app_blinds STATIC app.c)

target_include_directories(app_blinds PUBLIC ../include/)

target_link_libraries(app_blinds
    pico_stdlib
    pico_time
    hardware_sync
    hardware_watchdog
    pico_cyw43_arch_none
    hardware_gpio
    hardware_timer
    hardware_irq
    # hardware_exception
    mqtt_client_lib
    # ADD HERE NEWS LIBRARIES #
    motor_lib
    blind_lib
    )

##### -------- ADD HERE NEW LIBRARIES ------- #####
##### Like lightbulb_lib, add the library here #####

# Motor library
add_library(motor_lib STATIC stepper_motor.c)

target_include_directories(motor_lib PUBLIC ../include/)

target_link_libraries(motor_lib
    pico_stdlib
    pico_time
    pico_flash
    hardware_timer
    hardware_gpio
    hardware_flash
    hardware_exception
    hardware_sync
    hardware_irq
    blind_lib
    )

# Blind library
add_library(blind_lib STATIC blind.c)

target_include_directories(blind_lib PUBLIC ../include/)

target_link_libraries(blind_lib
    pico_stdlib
    pico_time
    pico_flash
    hardware_gpio
    hardware_timer
    hardware_flash
    hardware_exception
    hardware_sync
    hardware_irq
    motor_lib
    )

