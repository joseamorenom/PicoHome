# APP library
add_library(app STATIC app.c)

target_include_directories(app PUBLIC ../include/)

target_link_libraries(app
                      pico_stdlib
                      mqtt_client_lib
                      pico_time
                      hardware_sync
                      hardware_pwm
                      )

# Lightbulb library
add_library(lightbulb_lib STATIC lightbulb.c)

target_include_directories(lightbulb_lib PUBLIC ../include/)

target_link_libraries(lightbulb_lib
                      pico_stdlib
                      mqtt_client_lib
                      hardware_pwm
                      )

# Sensor KY018 library
add_library(sen_ky018_lib STATIC lightbulb.c)

target_include_directories(sen_ky018_lib PUBLIC ../include/)

target_link_libraries(sen_ky018_lib
                    pico_stdlib
                    mqtt_client_lib
                    hardware_adc
                    )

