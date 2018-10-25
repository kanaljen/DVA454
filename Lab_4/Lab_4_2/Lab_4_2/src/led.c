/*
 * led.c
 *
 * Created: 2018-10-25 16:16:33
 *  Author: sbn14007
 */ 

#include "led.h"

void LED_init(void)
{
	/* Select the pin we are going to use as GPIO */
	GPIO_PORT1_GPERS = (1 << 27);
	/* Set the value of that pin to HIGH */
	GPIO_PORT1_OVRS = (1 << 27);
	/* Enable the corresponding driver for that pin */
	GPIO_PORT1_ODERS = (1 << 27);
	
}

void LED_set(uint8_t value)
{
	if(value != 0)
	{
		/* Set the value on the LED0 pin to HIGH */
		GPIO_PORT1_OVRC = (0x01 << 27);
	}
	else
	{
		/* Set the value on the LED0 pin to LOW */
		GPIO_PORT1_OVRS = (0x01 << 27);
	}
}


