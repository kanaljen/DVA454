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
	
	// Task Handles
	xTaskHandle *hndBlinker = malloc(sizeof(xTaskHandle*)*3);
	
	// Create semaphore
	xSemaphoreHandle *hndSem = malloc(sizeof(xSemaphoreHandle*));
	vSemaphoreCreateBinary(hndSem);
	
	// Create tasks
	xTaskCreate(tskBlinking0,"tskBlinking0",STACK_SIZE,&hndSem,tskIDLE_PRIORITY+1,&(hndBlinker[0]));
	xTaskCreate(tskBlinking1,"tskBlinking1",STACK_SIZE,&hndSem,tskIDLE_PRIORITY+2,&(hndBlinker[0]));
	xTaskCreate(tskBlinking2,"tskBlinking2",STACK_SIZE,&hndSem,tskIDLE_PRIORITY+3,&(hndBlinker[0]));
	xTaskCreate(tskButtons,"tskButtons",STACK_SIZE,hndBlinker,1,NULL);
	
	// Start Scheduler       
	vTaskStartScheduler();
	
	// Keep main alive!
	while(TRUE);

}
