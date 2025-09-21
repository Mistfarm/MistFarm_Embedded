/*
 * Sensor.h
 *
 *  Created on: 2025. 9. 7.
 *      Author: user
 */

#include "sensor/sensor.h"
#include "driver/i2c.h"
#include "driver/adc.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "hal/adc_types.h"
#include <math.h>

#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

#define SHT45_ADDR 0x44
#define TAG "SENSOR"

// NTC 설정
#define NTC_ADC_CHANNEL ADC1_CHANNEL_6 // GPIO34
#define NTC_SERIES_RESISTOR 10000.0    // 직렬 저항 10k
#define NTC_NOMINAL 10000.0            // NTC 저항값 @25℃
#define TEMP_NOMINAL 25.0
#define B_COEFFICIENT 3435.0           // B 값

// SHT45 (습도 센서) 초기화
void humi_init() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    // NTC 초기화도 같이 처리
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(NTC_ADC_CHANNEL, ADC_ATTEN_DB_12);
}

// 습도 읽기 (SHT45)
float get_sht45_humidity() {
    uint8_t cmd[] = { 0x24, 0x00 }; // High repeatability
    uint8_t data[6];

    // 측정 명령 전송
    i2c_master_write_to_device(I2C_MASTER_NUM, SHT45_ADDR, cmd, 2, pdMS_TO_TICKS(100));
    vTaskDelay(pdMS_TO_TICKS(20));

    // 측정값 읽기
    i2c_master_read_from_device(I2C_MASTER_NUM, SHT45_ADDR, data, 6, pdMS_TO_TICKS(100));

    // 습도 파싱
    uint16_t humi_raw = (data[3] << 8) | data[4];
    float humidity = 100.0f * humi_raw / 65535.0f;
    return humidity;
}

// 온도 읽기 (NTC 서미스터)
float get_ntc_temperature() {
    int adc = adc1_get_raw(NTC_ADC_CHANNEL);
    if (adc <= 0) return -100.0; // 오류 처리

    float resistance = ((4095.0 / adc) - 1.0) * NTC_SERIES_RESISTOR;

    float steinhart;
    steinhart = resistance / NTC_NOMINAL;
    steinhart = log(steinhart);
    steinhart /= B_COEFFICIENT;
    steinhart += 1.0 / (TEMP_NOMINAL + 273.15);
    steinhart = 1.0 / steinhart;
    steinhart -= 273.15;

    return steinhart;
}