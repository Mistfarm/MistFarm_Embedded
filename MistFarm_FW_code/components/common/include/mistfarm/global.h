/*
 * global.h
 *
 *  Created on: 2025. 9. 7.
 *      Author: user
 */

#ifndef COMPONENTS_GLOBAL_H_
#define COMPONENTS_GLOBAL_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "esp_websocket_client.h"
#include "esp_event_base.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "driver/uart.h"
#include "cJSON.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"

//ota_update
#include "esp_ota_ops.h"    
#include "esp_http_client.h"   
#include "esp_https_ota.h"     
#include "esp_system.h"  

//sensor
#include "driver/i2c.h"
#include "driver/adc.h"
#include "hal/adc_types.h"
#include <math.h>

#endif /* COMPONENTS_GLOBAL_H_ */
