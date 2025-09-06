/*
 * GPS.h
 *
 *  Created on: 2025. 9. 7.
 *      Author: user
 */

#include "GPS.h"

static double convert_to_decimal_degrees(const char* nmea_coord, char direction) {
    if (nmea_coord == NULL || *nmea_coord == '\0') {
        return 0.0;
    }
    
    double raw_coord = atof(nmea_coord);
    int degrees = (int)(raw_coord / 100);
    double minutes = raw_coord - (degrees * 100);
    double decimal_degrees = degrees + (minutes / 60.0);

    if (direction == 'S' || direction == 'W') {
        decimal_degrees = -decimal_degrees;
    }
    
    return decimal_degrees;
}

/**
 * @brief GPRMC 문장을 파싱하여 save_status 구조체를 업데이트하는 함수
 * @param nmea_sentence 파싱할 NMEA 문장
 * @return 데이터가 유효하면 true, 아니면 false
 */
static bool parse_gprmc_and_update_status(char *nmea_sentence) {
    char *tokens[15];
    int i = 0;

    // NMEA 문장을 ',' 기준으로 분리
    char *token = strtok(nmea_sentence, ",");
    while (token != NULL && i < 15) {
        tokens[i++] = token;
    }

    // GPRMC 문장은 최소 7개의 필드를 가져야 위도/경도 정보를 포함함
    if (i < 7) {
        return false;
    }

    // 데이터 유효성 검사 (A = Active, V = Void)
    if (strcmp(tokens[2], "A") == 0) {
        // 위도 및 남/북반구 정보를 save_status.latit에 저장
        save_status.latit = convert_to_decimal_degrees(tokens[3], tokens[4][0]);

        // 경도 및 동/서반구 정보를 save_status.longi에 저장
        save_status.longi = convert_to_decimal_degrees(tokens[5], tokens[6][0]);
        
        return true; // 데이터 유효함
    }

    return false; // 데이터 유효하지 않음
}


/**
 * @brief GPS 모듈과 통신할 UART 초기화
 */
void GPS_init() {
    const uart_config_t uart_config = {
        .baud_rate = GPS_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    uart_driver_install(GPS_UART_PORT, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(GPS_UART_PORT, &uart_config);
    uart_set_pin(GPS_UART_PORT, GPS_TX_PIN, GPS_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    ESP_LOGI(TAG, "UART for GPS initialized.");
}

/**
 * @brief GPS 데이터를 읽고 파싱하여 save_status를 업데이트하는 FreeRTOS 태스크
 */
void GPS_task(void* pvParameters) {
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE + 1);

    while (1) {
        const int rxBytes = uart_read_bytes(GPS_UART_PORT, data, RX_BUF_SIZE, 200 / portTICK_PERIOD_MS);
        
        if (rxBytes > 0) {
            data[rxBytes] = 0; 
            
            char *line = strtok((char*)data, "\r\n");
            while(line != NULL) {
                // $GPRMC 또는 $GNRMC 문장만 파싱 (L86은 주로 GNRMC 사용)
                if (strncmp(line, "$GPRMC", 6) == 0 || strncmp(line, "$GNRMC", 6) == 0) {
                    
                    // 파싱 함수를 호출하여 save_status를 직접 업데이트
                    bool is_valid = parse_gprmc_and_update_status(line);

                    if (is_valid) {
                        ESP_LOGI(TAG, "GPS data updated! Lat: %f, Lon: %f", 
                                 save_status.latit, save_status.longi);
                    } else {
                        ESP_LOGW(TAG, "GPS data not valid yet. Waiting for fix...");
                    }
                }
                line = strtok(NULL, "\r\n");
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    free(data);
}