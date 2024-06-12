# APP library
add_library(app_blinds STATIC app.c)

target_include_directories(app_blinds PUBLIC ../include/)

target_link_libraries(app_blinds
    pico_stdlib
    pico_time
    hardware_sync
    hardware_watchdog
    mqtt_client_lib
    # ADD HERE NEWS LIBRARIES #
    # ej: lightbulb_lib
    )

##### -------- ADD HERE NEW LIBRARIES ------- #####
##### Like lightbulb_lib, add the library here #####


# # Lightbulb library
# add_library(lightbulb_lib STATIC lightbulb.c)

# target_include_directories(lightbulb_lib PUBLIC ../include/)

# target_link_libraries(lightbulb_lib
#     pico_stdlib
#     hardware_pwm
#     )

