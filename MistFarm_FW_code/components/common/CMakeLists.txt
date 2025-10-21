idf_component_register(
    SRCS "common.c"
    INCLUDE_DIRS "include"
    REQUIRES 
    	esp_http_client 
        esp_websocket_client
        freertos 
        driver 
        esp_event 
        nvs_flash 
        cjson
        log
        esp_wifi
        esp_system
        esp_netif
        app_update
        esp_https_ota
        hal
)
