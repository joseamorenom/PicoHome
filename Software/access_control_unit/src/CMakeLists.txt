# APP library
add_library(app_access STATIC app.c)

target_include_directories(app_access PUBLIC ../include/)

target_link_libraries(app_access
    pico_stdlib
    pico_time
    hardware_sync
    mqtt_client_lib
    keypad_lib
    nfc_lib
    )

# Keypad library
add_library(keypad_lib STATIC keypad_irq.c)

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
    hardware_gpio
    hardware_spi
    hardware_irq
    )

