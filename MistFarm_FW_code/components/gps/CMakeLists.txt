idf_component_register(
    SRCS "GPS.c"
    INCLUDE_DIRS "include"
    REQUIRES
        common
        driver
        freertos
        esp_event
        log
)
