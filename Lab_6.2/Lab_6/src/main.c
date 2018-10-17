#include "functions.h"

int main(void)
{	
	xTaskHandle *ProducerTaskHandle = malloc(sizeof(xTaskHandle*));
	xTaskHandle *ConsumerTaskHandle = malloc(sizeof(xTaskHandle*));
	USART_init();
	
	//Initiate display
	display_init();
	dip204_set_cursor_position(1, 1);
	dip204_printf_string("HELLO WORLD!");
	
	xTaskCreate(vSemaphoreTask, "xsemTask0", 256, NULL, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(vProducerTask, "xTask0", 256, &ConsumerTaskHandle, tskIDLE_PRIORITY, &ProducerTaskHandle);
	xTaskCreate(vConsumerTask, "xTask1", 256, &ProducerTaskHandle, tskIDLE_PRIORITY, &ConsumerTaskHandle);
	usart_write_line(configDBG_USART , "Starting Scheduler\n");
	vTaskStartScheduler();
	while(1);

}
