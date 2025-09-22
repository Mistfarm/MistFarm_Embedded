/*
 * wifi.h
 *
 *  Created on: 2025. 9. 7.
 *      Author: user
 */

#ifndef COMPONENTS_WIFI_H_
#define COMPONENTS_WIFI_H_

#include "mistfarm/config.h"

//#include "freertos/FreeRTOS.h"
//#include "freertos/event_groups.h"
//#include "esp_wifi.h"
//#include "esp_event.h"
//#include "esp_log.h"
//#include "esp_system.h"
//#include "nvs_flash.h""
//#include "esp_netif.h"

// Wi-Fi 연결 완료 시 세트될 비트
#define WIFI_CONNECTED_BIT BIT0

// 다른 파일에서 접근할 이벤트 그룹 핸들러 (예: WebSocket 연결 조건으로 사용)
extern EventGroupHandle_t wifi_event_group;

// Wi-Fi STA 모드 초기화 함수
void wifi_init_sta(void);

#endif /* COMPONENTS_WIFI_H_ */

