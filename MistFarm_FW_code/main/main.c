#include "gps/GPS.h"
#include "wss/WSS.h"
#include "mistfarm/config.h"
#include "freertos/event_groups.h"  
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
void control_task(void *pvParameters);

void app_main(void)
{
    wifi_event_group = xEventGroupCreate();

    xTaskCreate(GPS_task, "gps_task", 4096, NULL, 5, NULL);
    xTaskCreate(wss_task, "wss_task", 4096, NULL, 5, NULL);
    xTaskCreate(control_task, "control_task", 4096, NULL, 5, NULL); 

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(10000)); // 10초마다 깨어남
    }
}
