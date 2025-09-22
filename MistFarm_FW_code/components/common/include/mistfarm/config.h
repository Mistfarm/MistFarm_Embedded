/*
 * config.h
 *
 * Created on: 2025. 9. 6.
 * Author: user
 */

#ifndef COMPONENTS_CONFIG_H_
#define COMPONENTS_CONFIG_H_

#include "mistfarm/global.h"

#define AC_TRIAC_SWITCHING 27
#define TEMP_SENSOR 25
#define HUMI_I2C_SDL 21
#define HUMI_I2C_SCL 22

#define GPS_RX_PIN         17
#define GPS_TX_PIN         16
#define GPS_UART_PORT      UART_NUM_2
#define GPS_BAUD_RATE      9600
#define RX_BUF_SIZE        1024

#define HOST_IP_ADDR "https://flp24.com"
#define PORT "443"
#define WIFI_CONNECTED_BIT BIT0
#define DNS_PORT 53
#define DNS_MAX_LEN 256

#define NTC_ADC_CHANNEL       ADC1_CHANNEL_6  // GPIO34
#define NTC_SERIES_RESISTOR   10000.0
#define NTC_NOMINAL           10000.0
#define TEMP_NOMINAL          25.0
#define B_COEFFICIENT         3435.0

#define I2C_MASTER_SCL_IO 22
#define I2C_MASTER_SDA_IO 21
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 100000

#define SHT45_ADDR 0x44

typedef struct PresentStatus{
	
  const char *device_id;//고유 ID
  double latit; // 위도
  double longi; // 경도
  double temp;
  double humi;
  uint8_t on_time;
  uint8_t off_time;
}PresentStatus;

extern PresentStatus save_status;
extern const char *GPS_TAG;
extern const char *WSS_TAG;
extern EventGroupHandle_t wifi_event_group;

#endif /* COMPONENTS_CONFIG_H_ */