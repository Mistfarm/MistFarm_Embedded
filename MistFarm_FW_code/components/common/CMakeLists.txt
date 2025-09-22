idf_component_register(
    SRCS "common.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        esp_websocket_client
        freertos 
        driver 
        esp_event 
        nvs_flash 
        cjson
        esp_log
        esp_wifi
        esp_system
        esp_netif
)
