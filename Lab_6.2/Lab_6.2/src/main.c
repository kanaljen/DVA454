#include "functions.h"

int main(void)
{	
	xTaskHandle *ProducerTaskHandle = malloc(sizeof(xTaskHandle*));
	xTaskHandle *ConsumerTaskHandle = malloc(sizeof(xTaskHandle*));
	xSemaphoreHandle *xSem = malloc(sizeof(xSemaphoreHandle*));
	USART_init();
	
	xTaskCreate(vSemaphoreTask, "xsemTask0", 256, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(vProducerTask, "xTask0", 256, &ConsumerTaskHandle, tskIDLE_PRIORITY, &ProducerTaskHandle);
	xTaskCreate(vConsumerTask, "xTask1", 256, &ProducerTaskHandle, tskIDLE_PRIORITY, &ConsumerTaskHandle);
	usart_write_line(configDBG_USART , "Starting Scheduler\n");
	vTaskStartScheduler();
	while(1);

}
