/*
 * config.h
 *
 *  Created on: 2025. 9. 6.
 *      Author: user
 */

#ifndef COMPONENTS_CONFIG_H_
#define COMPONENTS_CONFIG_H_

#define AC_TRIAC_SWITCHING 27
#define TEMP_SENSOR 25
#define HUMI_I2C_SDL 21
#define HUMI_I2C_SCL 22
#define GPS_RX 17
#define GPS_TX 16

typedef struct PresentStatus{
  double latit; // 위도
  double longi; // 경도
  double temp;
  double humi;
}PresentStatus;

PresentStatus save_status;

#endif /* COMPONENTS_CONFIG_H_ */
