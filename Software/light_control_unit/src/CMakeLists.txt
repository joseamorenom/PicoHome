# APP library
add_library(app STATIC app.c)

target_include_directories(app PUBLIC ../include/)

target_link_libraries(app
    pico_stdlib
    pico_time
    hardware_sync
    mqtt_client_lib
    lightbulb_lib
    sen_ky018_lib
    )

# Lightbulb library
add_library(lightbulb_lib STATIC lightbulb.c)

target_include_directories(lightbulb_lib PUBLIC ../include/)

target_link_libraries(lightbulb_lib
    pico_stdlib
    hardware_pwm
    )

# Sensor KY018 library
add_library(sen_ky018_lib STATIC sen_ky018.c)

target_include_directories(sen_ky018_lib PUBLIC ../include/)

target_link_libraries(sen_ky018_lib
    pico_stdlib
    hardware_gpio
    hardware_adc
    )

