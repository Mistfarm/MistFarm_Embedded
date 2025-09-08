idf_component_register(
    SRCS "wifi.c"
    INCLUDE_DIRS "include"
    REQUIRES
        common
        nvs_flash
        esp_event
        esp_netif
        esp_wifi
        freertos
        log
)
