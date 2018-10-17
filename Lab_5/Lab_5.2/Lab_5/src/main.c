#include "functions.h"

int main(void)
{	
	volatile avr32_usart_t *usart = &AVR32_USART1;
	xSemaphoreHandle *xSem = malloc(sizeof(xSemaphoreHandle*)); //Semaphore handle
	
	USART_init(); 
	init_LED();
	
	vSemaphoreCreateBinary(xSem); //Create binary semaphore
	
	xTaskCreate(vLED_TASK0, "LED0", 256, &xSem, tskIDLE_PRIORITY+3, NULL); //High priority task
	xTaskCreate(vLED_TASK1, "LED1", 256, NULL, tskIDLE_PRIORITY+2, NULL); //Medium priority task
	xTaskCreate(vLED_TASK2, "LED2", 256, &xSem, tskIDLE_PRIORITY+1, NULL); //Low priority task
	usart_write_line(configDBG_USART , "Starting Scheduler\n");
	vTaskStartScheduler();
	while(1);

}
