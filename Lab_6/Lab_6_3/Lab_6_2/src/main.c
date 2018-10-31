/*
 * main.c
 *
 * Created: 2018-10-30 16:00:08
 *  Author: sbn14007
 */ 

#include "define.h"

int main(void){

	USART_init();
	initSemaphore();
	display_init();
	xTaskCreate(tskReceiver, "RECITSK", STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(tskStatus, "STATUSTSK", STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(tskButton, "BUTTONTSK", STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	
	vTaskStartScheduler();

	while(TRUE);
}