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
	xTaskHandle *blinker = malloc(sizeof(xTaskHandle*)*3);
	
	// Task Info
	char* tskNames = {"tskBlinking0","tskBlinking1","tskBlinking2"};
	int led[3] = {0,1,2};
	
	// Create tasks
	for(int i=0;i<3;i++)xTaskCreate(tskBlinking,&tskNames[i],STACK_SIZE,&led[i],1,&(blinker[i]));
	
	xTaskCreate(tskButtons,"tskButtons",STACK_SIZE,blinker,1,NULL);
	
	// Start Scheduler       
	vTaskStartScheduler();
	
	// Keep main alive!
	while(TRUE);

}
