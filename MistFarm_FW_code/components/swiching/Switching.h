/*
 * Switching.h
 *
 *  Created on: 2025. 9. 7.
 *      Author: user
 */

#ifndef COMPONENTS_SWITCHING_H_
#define COMPONENTS_SWITCHING_H_
#include "../config.h"

void GPIO_init();
void on_off_time_set(); // 인터럽트로 개발
void switching_task(void* pvParameter);

#endif /* COMPONENTS_SWITCHING_H_ */
