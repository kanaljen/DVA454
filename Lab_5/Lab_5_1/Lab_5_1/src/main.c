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
	
	// Create tasks
	xTaskCreate(vBlinkLED0,"LED0BLINK",STACK_SIZE,NULL,1,blinker[0]);
	xTaskCreate(vBlinkLED1,"LED1BLINK",STACK_SIZE,NULL,1,blinker[1]);
	xTaskCreate(vBlinkLED2,"LED2BLINK",STACK_SIZE,NULL,1,blinker[2]);
	xTaskCreate(vButtons,"BUTTONS",STACK_SIZE,NULL,1,NULL);
	
	// Start Scheduler       
	vTaskStartScheduler();
	
	// Keep main alive!
	while(TRUE);

}
