#include "wss/WSS.h"
#include "esp_websocket_client.h"
#include "freertos/projdefs.h"
#include "mistfarm/config.h"
#include "portmacro.h"
#include "cJSON.h"

static char* build_status_json(PresentStatus *status) {
    cJSON *root = cJSON_CreateObject();
    if (!root) return NULL;

    cJSON_AddStringToObject(root, "device_id", status->device_id);
    cJSON_AddNumberToObject(root, "latit", status->latit);
    cJSON_AddNumberToObject(root, "longi", status->longi);
    cJSON_AddNumberToObject(root, "temp", status->temp);
    cJSON_AddNumberToObject(root, "humi", status->humi);
    cJSON_AddNumberToObject(root, "on_time", status->on_time);
    cJSON_AddNumberToObject(root, "off_time", status->off_time);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    return json_str; // 호출 후 free()
}


void websocket_event_handler(void *handler_args, esp_event_base_t base,
                                    int32_t event_id, void *event_data) {
    esp_websocket_event_data_t *event = (esp_websocket_event_data_t *)event_data;

    switch (event_id) {
        case WEBSOCKET_EVENT_CONNECTED:
            ESP_LOGI(WSS_TAG, "WebSocket connected");
            break;
        case WEBSOCKET_EVENT_DISCONNECTED:
            ESP_LOGI(WSS_TAG, "WebSocket disconnected");
            break;
        case WEBSOCKET_EVENT_DATA:
            ESP_LOGI(WSS_TAG, "Received data: %.*s", event->data_len, (char *)event->data_ptr);

            char *rx_data = strndup((char *)event->data_ptr, event->data_len);
            if (rx_data) {
                cJSON *root = cJSON_Parse(rx_data);
                if (root) {
                    // 데이터 받는 코드
                    cJSON *cmd = cJSON_GetObjectItem(root, "command");
    				if (cJSON_IsString(cmd)) {
        				if (strcmp(cmd->valuestring, "update") == 0) {
            				cJSON *on_time = cJSON_GetObjectItem(root, "on_time");
            				cJSON *off_time = cJSON_GetObjectItem(root, "off_time");

            				if (cJSON_IsNumber(on_time)) save_status.on_time = on_time->valueint;
            				if (cJSON_IsNumber(off_time)) save_status.off_time = off_time->valueint;

            				ESP_LOGI(WSS_TAG, "Updated status: on_time=%d, off_time=%d",
                    				 save_status.on_time, save_status.off_time);
        				}
    				}
    				cJSON_Delete(root);

                }
                free(rx_data);
            }
            break;
        case WEBSOCKET_EVENT_ERROR:
            ESP_LOGE(WSS_TAG, "WebSocket error occurred");
            break;
    }
}

void wss_task(void *pvParameters) {
    esp_websocket_client_config_t websocket_cfg = {
        .uri = "wss://" HOST_IP_ADDR ":" PORT "/",
        .network_timeout_ms = 10000,
    };
    esp_websocket_client_handle_t client = esp_websocket_client_init(&websocket_cfg);
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, (void *)client);
    
    esp_websocket_client_start(client);
    while (1) {

        if (esp_websocket_client_is_connected(client)) {
			// 코드 작성 요함(전송코드)
			ESP_LOGI(WSS_TAG, "연결됨");
			static TickType_t last_tick = 0;
			if ((xTaskGetTickCount() - last_tick) > pdMS_TO_TICKS(5000)){
				const char *msg = "{\"type\":\"heartbeat\", \"data\":\"hello\"}";
				esp_websocket_client_send_text(client,msg,strlen(msg),portMAX_DELAY);
				ESP_LOGI(WSS_TAG, "서버로 전송 : %s",msg);
				last_tick = xTaskGetTickCount();
			}
        }

        else {
            EventBits_t bits = xEventGroupGetBits(wifi_event_group);
            if (bits & WIFI_CONNECTED_BIT) {
                ESP_LOGI(WSS_TAG, "WebSocket disconnected. Attempting to reconnect...");
                esp_websocket_client_start(client); // 재연결 시도
            } else {
                ESP_LOGW(WSS_TAG, "Wi-Fi not connected. Waiting for Wi-Fi connection...");
            }
            vTaskDelay(pdMS_TO_TICKS(5000));
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    esp_websocket_client_stop(client);
    esp_websocket_client_destroy(client);
    vTaskDelete(NULL);
}
