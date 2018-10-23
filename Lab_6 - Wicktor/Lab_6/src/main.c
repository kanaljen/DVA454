#include "functions.h"

int main(void)
{	
	xTaskHandle *ProducerTaskHandle = malloc(sizeof(xTaskHandle*));
	xTaskHandle *ConsumerTaskHandle = malloc(sizeof(xTaskHandle*));
	usart_init();
	display_init();
	sensor_init(); 	
	queue_init();
	
	xTaskCreate(vPotTask, "Pot", 256, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vTempTask, "Temp", 256, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vLSTask, "LS", 256, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vDisplayTask, "Dis", 256, NULL, tskIDLE_PRIORITY, NULL);
	
	usart_write_line(configDBG_USART , "Starting Scheduler\n");
	vTaskStartScheduler();
}
