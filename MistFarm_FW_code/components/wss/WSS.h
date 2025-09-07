/*
 * WSS.h
 *
 *  Created on: 2025. 9. 7.
 *      Author: user
 */

#ifndef COMPONENTS_WSS_H_
#define COMPONENTS_WSS_H_
#include "../config.h"

void wss_task(void* pvPrameter);
static void websocket_event_handler(void *handler_args, esp_event_base_t base,
                                    int32_t event_id, void *event_data);

#endif /* COMPONENTS_WSS_H_ */
