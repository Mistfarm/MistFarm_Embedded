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

typedef struct PresentStatus{
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