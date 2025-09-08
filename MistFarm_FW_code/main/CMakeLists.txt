idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES
        common
        gps
        wss
        wifi
        sensor
        switching
        cjson
)
