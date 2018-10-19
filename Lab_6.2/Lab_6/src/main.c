#include "functions.h"

int main(void)
{	
	xTaskHandle *LCDTaskHandle = malloc(sizeof(xTaskHandle*));
	USART_init();
	
	//Initiate display
	display_init();

	xTaskCreate(vUSARTTask, "xUSARTTask", 256, &LCDTaskHandle, tskIDLE_PRIORITY+2, NULL);
	xTaskCreate(vLCDTask, "xLCDTask", 256, NULL, tskIDLE_PRIORITY+2, &LCDTaskHandle);
	xTaskCreate(vButtonTASK, "xButtonTask", 256, &LCDTaskHandle, tskIDLE_PRIORITY+2, NULL);

	usart_write_line(configDBG_USART , "Starting Scheduler\n");	
	vTaskStartScheduler();
	while(1);

}
