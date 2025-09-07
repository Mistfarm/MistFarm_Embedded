/*
 * GPS.h
 *
 *  Created on: 2025. 9. 7.
 *      Author: user
 */

#ifndef COMPONENTS_GPS_H_
#define COMPONENTS_GPS_H_
#include "../config.h"

static double convert_to_decimal_degrees(const char* nmea_coord, char direction);
static bool parse_gprmc_and_update_status(char *nmea_sentence);
void GPS_init();
void GPS_task(void* pvParameters);

#endif /* COMPONENTS_GPS_H_ */
