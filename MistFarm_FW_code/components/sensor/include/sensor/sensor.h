/*
 * Sensor.h
 *
 *  Created on: 2025. 9. 7.
 *      Author: user
 */

#ifndef COMPONENTS_SENSOR_H_
#define COMPONENTS_SENSOR_H_
#include "mistfarm/config.h"

// Sensor.h
float get_sht45_humidity(void);
float get_ntc_temperature(void);


void humi_init();
void env_task(void* pvParameter);


#endif /* COMPONENTS_SENSOR_H_ */

