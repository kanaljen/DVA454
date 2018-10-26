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
	
	// Create tasks
	xTaskCreate(vBlinkLED0,"LED0TASK",STACK_SIZE,NULL,1,NULL);
	xTaskCreate(vBlinkLED1,"LED0TASK",STACK_SIZE,NULL,1,NULL);
	xTaskCreate(vBlinkLED2,"LED0TASK",STACK_SIZE,NULL,1,NULL);
	
	// Start Scheduler       
	vTaskStartScheduler();
	
	// Keep main alive!
	while(TRUE);

}
