idf_component_register(
	SRCS "ota_update.c"
    INCLUDE_DIRS "include"
    REQUIRES 
        common
    	#esp_https_ota
    	#app_update
    	#esp_http_client
    	#esp_system
    	#esp_websocket_client  
    	#esp_event
    	#freertos
    	#driver
    	#cjson
    	#esp_wifi
    	#nvs_flash
    	#esp_netif
    	#log
    	
)