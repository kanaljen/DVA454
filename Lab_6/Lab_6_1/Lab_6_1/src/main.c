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
	
	xTaskCreate(tskProducer, "Producer", 256, &ConsumerTaskHandle, STACK_SIZE, &ProdTaskHandle);
	xTaskCreate(tskConsumer, "Consumer", 256, &ProducerTaskHandle, tskIDLE_PRIORITY, &ConsTaskHandle);
	
}