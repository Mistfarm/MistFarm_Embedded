/*
 * GPS.h
 *
 *  Created on: 2025. 9. 7.
 *      Author: user
 */

#include "gps/GPS.h"

double convert_to_decimal_degrees(const char* nmea_coord, char direction) {
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

bool parse_gprmc_and_update_status(char *nmea_sentence) {
    char *tokens[15];
    int i = 0;

    char *token = strtok(nmea_sentence, ",");
    while (token != NULL && i < 15) {
        tokens[i++] = token;
    }

    if (i < 7) {
        return false;
    }

    if (strcmp(tokens[2], "A") == 0) {
        save_status.latit = convert_to_decimal_degrees(tokens[3], tokens[4][0]);

        save_status.longi = convert_to_decimal_degrees(tokens[5], tokens[6][0]);
        
        return true;
    }

    return false;
}

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
    ESP_LOGI(GPS_TAG, "UART for GPS initialized.");
}

void GPS_task(void* pvParameters) {
    uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE + 1);

    while (1) {
        const int rxBytes = uart_read_bytes(GPS_UART_PORT, data, RX_BUF_SIZE, 200 / portTICK_PERIOD_MS);
        
        if (rxBytes > 0) {
            data[rxBytes] = 0; 
            
            char *line = strtok((char*)data, "\r\n");
            while(line != NULL) {
                if (strncmp(line, "$GPRMC", 6) == 0 || strncmp(line, "$GNRMC", 6) == 0) {
                    
                    bool is_valid = parse_gprmc_and_update_status(line);

                    if (is_valid) {
                        ESP_LOGI(GPS_TAG, "GPS data updated! Lat: %f, Lon: %f", 
                                 save_status.latit, save_status.longi);
                    } else {
                        ESP_LOGW(GPS_TAG, "GPS data not valid yet. Waiting for fix...");
                    }
                }
                line = strtok(NULL, "\r\n");
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    free(data);
}