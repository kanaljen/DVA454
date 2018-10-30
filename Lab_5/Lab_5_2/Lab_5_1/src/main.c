/*
 * main.c
 *
 * Created: 2018-10-26 14:57:35
 *  Author: sbn14007
 */ 

#include "functions.h"	// Functions for task

int main (void){
	
	// Init functions
	USART_init();
	LED_init();
	
	//Create Semaphore
	xSemaphoreHandle xSemaphore;
	vSemaphoreCreateBinary(xSemaphore);

	// Create tasks
	xTaskCreate(tskHigh,"tskHigh",STACK_SIZE,&xSemaphore,tskIDLE_PRIORITY+3,NULL);
	xTaskCreate(tskMed,"tskMed",STACK_SIZE,&xSemaphore,tskIDLE_PRIORITY+2,NULL);
	xTaskCreate(tskLow,"tskLow",STACK_SIZE,&xSemaphore,tskIDLE_PRIORITY+1,NULL);
	
	// Start Scheduler       
	vTaskStartScheduler();
	
	// Keep main alive!
	while(TRUE);

}
