/*
 * main.c
 *
 * Created: 2018-10-26 14:57:35
 *  Author: sbn14007
 */ 

#include "functions.h"	// Functions for task

int main (void){
	
	USART_init();
	LED_init();

/*
	xTaskCreate(
	vTaskCode,       
	"NAME",         
	STACK_SIZE,     
	( void * ) 1,    
	tskIDLE_PRIORITY,
	&xHandle );      
*/

}
