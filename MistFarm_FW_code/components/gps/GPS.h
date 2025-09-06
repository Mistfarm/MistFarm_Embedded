/*
 * GPS.h
 *
 *  Created on: 2025. 9. 7.
 *      Author: user
 */

#ifndef COMPONENTS_GPS_H_
#define COMPONENTS_GPS_H_
#include "../config.h"
#include "../global.h"

void GPS_init();
void GPS_rerode();
void GPS_task(void* pvParameters);


#endif /* COMPONENTS_GPS_H_ */
