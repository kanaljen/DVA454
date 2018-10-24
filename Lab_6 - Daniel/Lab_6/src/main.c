#include "functions.h"

int main(void)
{	
	usart_init();
	display_init();
	SENSOR_init(); 	
	QUEUE_init();
	
	xTaskCreate(vQueueTASK, "Que", 256, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vPotTASK, "Pot", 256, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vTmpTASK, "Tmp", 256, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vLdrTASK, "Ldr", 256, NULL, tskIDLE_PRIORITY, NULL);
	
	usart_write_line(configDBG_USART , "Starting Scheduler\n");
	vTaskStartScheduler();
}
