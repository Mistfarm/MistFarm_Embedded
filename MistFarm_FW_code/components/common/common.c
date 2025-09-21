/*
 * common.c
 *
 * Created on: 2025. 9. 6.
 * Author: user
 */

#include "mistfarm/config.h"

PresentStatus save_status = {
	.latit = 0,
	.longi = 0,
	.temp = 25,
	.humi = 50,
	.on_time = 10, // 단위 second
	.off_time = 10
};

const char *GPS_TAG = "GPS";
const char *WSS_TAG = "WSS";

extern EventGroupHandle_t wifi_event_group;