idf_component_register(
    SRCS "switching.c"
    INCLUDE_DIRS "include"
    REQUIRES
        common
        driver
        freertos
        log
)
