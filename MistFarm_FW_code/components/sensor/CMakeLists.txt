idf_component_register(
    SRCS "sensor.c"
    INCLUDE_DIRS "include"
    REQUIRES
        common
        esp_websocket_client
		freertos
		driver
		esp_event
		nvs_flash
		cjson
        log
)
