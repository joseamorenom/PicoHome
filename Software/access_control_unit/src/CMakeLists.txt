# APP library
add_library(app_access STATIC app.c)

target_include_directories(app_access PUBLIC ../include/)

target_link_libraries(app_access
    pico_stdlib
    pico_time
    pico_flash
    hardware_pwm
    hardware_sync
    hardware_watchdog
    mqtt_client_lib
    keypad_lib
    nfc_lib
    pir_lib
    door_lib
    )

# Keypad library
add_library(keypad_lib STATIC keypad.c)

target_include_directories(keypad_lib PUBLIC ../include/)

target_link_libraries(keypad_lib
    pico_stdlib
    pico_time
    hardware_gpio
    hardware_pwm
    app_access
    )

# NFC library
add_library(nfc_lib STATIC nfc_rfid.c)

target_include_directories(nfc_lib PUBLIC ../include/)

target_link_libraries(nfc_lib
    pico_stdlib
    pico_time
    pico_flash
    hardware_flash
    hardware_exception
    hardware_sync
    hardware_gpio
    hardware_spi
    hardware_irq
    )

# PIR library
add_library(pir_lib STATIC pir.c)

target_include_directories(pir_lib PUBLIC ../include/)

target_link_libraries(pir_lib
    hardware_gpio
    app_access
    hardware_irq
    )

# PIR library
add_library(door_lib STATIC door.c)

target_include_directories(door_lib PUBLIC ../include/)

target_link_libraries(door_lib
    pico_time
    hardware_gpio
    app_access
    )


