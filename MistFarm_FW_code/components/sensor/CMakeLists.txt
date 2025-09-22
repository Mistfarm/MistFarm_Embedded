idf_component_register(

SRCS "common.c" # 비어 있어도 최소 1개 필요

INCLUDE_DIRS "include" # include/mistfarm/ 아래 노출

REQUIRES
	esp_websocket_client # config/global에서 사용
	freertos
	driver
	esp_event
	nvs_flash
	cjson


)