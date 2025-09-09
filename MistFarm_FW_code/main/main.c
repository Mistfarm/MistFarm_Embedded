#include "gps/GPS.h"
#include "wss/WSS.h"
#include "mistfarm/config.h"

void app_main(void)
{
	
	
	xTaskCreate(GPS_task, "gps_task", 4096, NULL, 5, NULL);
	xTaskCreate(wss_task, "wss_task", 4096, NULL, 5, NULL);
    while (true) {
		printf("hi there");
    }
}
