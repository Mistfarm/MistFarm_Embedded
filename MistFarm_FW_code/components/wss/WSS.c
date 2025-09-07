/*
 * WSS.h
 *
 *  Created on: 2025. 9. 7.
 *      Author: user
 */

#include "WSS.h"

static void websocket_event_handler(void *handler_args, esp_event_base_t base,
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
                    cJSON *danger_array = cJSON_GetObjectItem(root, "danger");
                    // 데이터 받는 코드
                }
                free(rx_data);
            }
            break;
        case WEBSOCKET_EVENT_ERROR:
            ESP_LOGE(WSS_TAG, "WebSocket error occurred");
            break;
    }
}

void wss_client_task(void *pv_parameters) {
    esp_websocket_client_config_t websocket_cfg = {
        .uri = "wss://" HOST_IP_ADDR ":" PORT "/",
        .network_timeout_ms = 10000,
    };
    esp_websocket_client_handle_t client = esp_websocket_client_init(&websocket_cfg);
    esp_websocket_register_events(client, WEBSOCKET_EVENT_ANY, websocket_event_handler, (void *)client);
    while (1) {

        if (esp_websocket_client_is_connected(client)) {
			// 코드 작성 요함
			ESP_LOGI(WSS_TAG, "연결됨");
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
