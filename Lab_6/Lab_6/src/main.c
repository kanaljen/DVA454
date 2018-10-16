#include "functions.h"

int main(void)
{	
	volatile avr32_usart_t *usart = &AVR32_USART1;
	xSemaphoreHandle *xSem = malloc(sizeof(xSemaphoreHandle*));
	
	USART_init();
	
	vSemaphoreCreateBinary(xSem);
	
	xTaskCreate(vLED_TASK0, "LED0", 256, &xSem, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vLED_TASK1, "LED1", 256, &xSem, tskIDLE_PRIORITY, NULL);
	usart_write_line(configDBG_USART , "Starting Scheduler\n");
	vTaskStartScheduler();
	while(1);

}
