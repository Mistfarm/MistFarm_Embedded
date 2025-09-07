#include "../components/gps/GPS.h"
#include "../components/wss/WSS.h"

void app_main(void)
{
	
	
	xTaskCreate(GPS_task, "gps_task", 4096, NULL, 5, NULL);
    while (true) {
		printf("hi there");
    }
}
