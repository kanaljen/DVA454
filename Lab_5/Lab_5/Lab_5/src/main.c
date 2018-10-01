#include "functions.h"

int main(void)
{	
	volatile avr32_usart_t *usart = &AVR32_USART1;
	xTaskHandle *blinkHandle = malloc(sizeof(xTaskHandle *)*3);
	
	USART_init();
	init_LED();
	xTaskCreate(vLED_TASK0, "LED0", 256, NULL, tskIDLE_PRIORITY+1, &(blinkHandle[0]));
	xTaskCreate(vLED_TASK1, "LED1", 256, NULL, tskIDLE_PRIORITY+1, &(blinkHandle[1]));
	xTaskCreate(vLED_TASK2, "LED2", 256, NULL, tskIDLE_PRIORITY+1, &(blinkHandle[2]));
	xTaskCreate(vbutton_TASK0, "BUTTON0", 256, &(blinkHandle[0]), tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(vbutton_TASK1, "BUTTON1", 256, &(blinkHandle[1]), tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(vbutton_TASK2, "BUTTON2", 256, &(blinkHandle[2]), tskIDLE_PRIORITY+1, NULL);
	usart_write_line(configDBG_USART , "Starting Scheduler\n"); 
	vTaskStartScheduler();	
	while(1);

}
