#include "functions.h"

int main(void)
{	
	xTaskHandle *CountTaskHandle = malloc(sizeof(xTaskHandle*));
	USART_init();
	
	//Initiate display
	display_init();

	xTaskCreate(vCountTASK, "xCountTask", 256, NULL, tskIDLE_PRIORITY, &CountTaskHandle);
	xTaskCreate(vLCDTask, "xLCDTask", 256, NULL, tskIDLE_PRIORITY, NULL);
	xTaskCreate(vButtonTASK, "xButtonTask", 256, &CountTaskHandle, tskIDLE_PRIORITY, NULL);

	usart_write_line(configDBG_USART , "Starting Scheduler\n");	
	vTaskStartScheduler();
	while(1);

}
