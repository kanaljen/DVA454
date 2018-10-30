/*
 * main.c
 *
 * Created: 2018-10-30 16:00:08
 *  Author: sbn14007
 */ 

#include "define.h"

int main(void){
	
	xTaskHandle *ProdTaskHandle;
	xTaskHandle *ConsTaskHandle;
	
	USART_init();
	LED_init();
	initSemaphore();
	
	xTaskCreate(tskProducer, "Producer", STACK_SIZE, &ConsTaskHandle, tskIDLE_PRIORITY+1, &ProdTaskHandle);
	xTaskCreate(tskConsumer, "Consumer", STACK_SIZE, &ProdTaskHandle, tskIDLE_PRIORITY+1, &ConsTaskHandle);
	
	vTaskStartScheduler();

	while(TRUE);
}