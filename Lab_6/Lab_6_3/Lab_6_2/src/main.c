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
	initQueues();
	
   // Configure the ADC module and enable the channels
   adc_configure(&AVR32_ADC);
   adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
   adc_enable(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
   adc_enable(&AVR32_ADC, ADC_LIGHT_CHANNEL);
	
	xTaskCreate(tskLight, "LIGHTTSK", STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(tskPotent, "POTENTTSK", STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(tskTemp, "TEMPTSK", STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	xTaskCreate(tskDisplay, "DISPTSK", STACK_SIZE, NULL, tskIDLE_PRIORITY+1,NULL);
	
	vTaskStartScheduler();

	while(TRUE);
}