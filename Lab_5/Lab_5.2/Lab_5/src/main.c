#include "functions.h"

int main(void)
{	
	volatile avr32_usart_t *usart = &AVR32_USART1;

	USART_init();
	init_LED();
	xTaskCreate(vLED_TASK0, "LED0", 256, NULL, tskIDLE_PRIORITY+3, NULL);
	xTaskCreate(vLED_TASK1, "LED1", 256, NULL, tskIDLE_PRIORITY+2, NULL);
	xTaskCreate(vLED_TASK2, "LED2", 256, NULL, tskIDLE_PRIORITY+1, NULL);
	usart_write_line(configDBG_USART , "Starting Scheduler\n"); 
	vTaskStartScheduler();	
	while(1);

}
