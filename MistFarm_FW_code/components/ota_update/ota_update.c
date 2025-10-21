#include  "ota_update/ota_update.h"

#define EXAMPLE_WIFI_SSID       "YOUR_WIFI_SSID"
#define EXAMPLE_WIFI_PASS       "YOUR_WIFI_PASSWORD"
#define OTA_FIRMWARE_URL        "https://your.ota.server.com/firmware.bin"

static const char *TAG = "OTA_EXAMPLE";
//static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;

/* Event handler for catching system events */
static void event_handler(void* arg, esp_event_base_t event_base,
                        int32_t event_id, void* event_data)
{
    if (event_base == ESP_HTTPS_OTA_EVENT) {
        switch (event_id) {
            case ESP_HTTPS_OTA_START:
                ESP_LOGI(TAG, "OTA started");
                break;
            case ESP_HTTPS_OTA_CONNECTED:
                ESP_LOGI(TAG, "Connected to server");
                break;
            case ESP_HTTPS_OTA_GET_IMG_DESC:
                ESP_LOGI(TAG, "Reading Image Description");
                break;
            case ESP_HTTPS_OTA_VERIFY_CHIP_ID:
                ESP_LOGI(TAG, "Verifying chip id of new image: %d", *(esp_chip_id_t *)event_data);
                break;
            case ESP_HTTPS_OTA_DECRYPT_CB:
                ESP_LOGI(TAG, "Callback to decrypt function");
                break;
            case ESP_HTTPS_OTA_WRITE_FLASH:
                ESP_LOGD(TAG, "Writing to flash: %d written", *(int *)event_data);
                break;
            case ESP_HTTPS_OTA_UPDATE_BOOT_PARTITION:
                ESP_LOGI(TAG, "Boot partition updated. Next Partition: %d", *(esp_partition_subtype_t *)event_data);
                break;
            case ESP_HTTPS_OTA_FINISH:
                ESP_LOGI(TAG, "OTA finish");
                break;
            case ESP_HTTPS_OTA_ABORT:
                ESP_LOGI(TAG, "OTA abort");
                break;
        }
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
        ESP_LOGI(TAG, "Wi-Fi Connected!");
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        ESP_LOGI(TAG, "Wi-Fi Disconnected. Retrying...");
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
    }
}
void ota_update_task(void *pvParameter)
{
    ESP_LOGI(TAG, "Waiting for Wi-Fi connection...");
    // Wi-Fi 연결이 완료될 때까지 대기
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

    ESP_LOGI(TAG, "Starting HTTPS OTA...");

    esp_http_client_config_t client_config = {
        .url = OTA_FIRMWARE_URL,
        // .cert_pem = (const char *)server_cert_pem_start, // HTTPS 필수 설정
        .timeout_ms = 5000,
    };

    esp_https_ota_config_t ota_config = {
        .http_config = &client_config,
    };

    esp_err_t ret = esp_https_ota(&ota_config);

    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "OTA update successful. Restarting...");
        esp_restart(); // 새 펌웨어로 부팅
    } else {
        ESP_LOGE(TAG, "OTA update failed! Error: %d", ret);
    }

    vTaskDelete(NULL); // 태스크 종료
}
void wifi_init(void)
{
    wifi_event_group = xEventGroupCreate();

    // TCP/IP 스택 초기화
    ESP_ERROR_CHECK(esp_netif_init());

    // 기본 이벤트 루프 생성
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Wi-Fi 이벤트 핸들러 등록
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
    
    // OTA 이벤트 핸들러 등록 (제시해 주신 핸들러가 여기서 작동합니다)
    ESP_ERROR_CHECK(esp_event_handler_register(ESP_HTTPS_OTA_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));

    // Wi-Fi 스테이션 설정
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_WIFI_SSID,
            .password = EXAMPLE_WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
}

void app_main(void)
{
    // NVS (Non-Volatile Storage) 초기화
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Wi-Fi 및 이벤트 시스템 초기화
    wifi_init();

    // OTA 업데이트를 처리할 태스크 시작
    xTaskCreate(&ota_update_task, "ota_update_task", 8192, NULL, 5, NULL);
}