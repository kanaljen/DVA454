/*
 * main.c
 *
 * Created: 2018-10-26 14:57:35
 *  Author: sbn14007
 */ 

#include "functions.h"	// Functions for task

int main (void){
	
	xTaskCreate(
	vTaskCode,       /* Function that implements the task. */
	"NAME",          /* Text name for the task. */
	STACK_SIZE,      /* Stack size in words, not bytes. */
	( void * ) 1,    /* Parameter passed into the task. */
	tskIDLE_PRIORITY,/* Priority at which the task is created. */
	&xHandle );      /* Used to pass out the created task's handle. */

}
