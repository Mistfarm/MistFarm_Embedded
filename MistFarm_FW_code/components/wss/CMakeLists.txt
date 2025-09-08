idf_component_register(
    SRCS "WSS.c"
    INCLUDE_DIRS "include"
    REQUIRES
        common
        esp_websocket_client
        freertos
        cjson
        esp_event
        log
)
