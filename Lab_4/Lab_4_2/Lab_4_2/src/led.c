/*
 * led.c
 *
 * Created: 2018-10-25 16:16:33
 *  Author: sbn14007
 */ 

#include "led.h"

void LED_init(void)
{
	/* Enable pin */
	GPIO1_GPERS |= (1 << LED_PIN); // GPIO Enable, page 178
	/* Pin is an OUTPUT */
	GPIO1_ODERS |= (1 << LED_PIN); // Output Driver Enable Register, page 180
	/* Set pin 27 */
	GPIO1_OVRS |= (1 << LED_PIN);  // Output Value Register, page 181
	
}

void LED_set(int bool)
{
	if(bool == TRUE)
	{

		GPIO1_OVRC = (0x01 << LED_PIN);  // LED ON
	}
	else
	{
		GPIO1_OVRS = (0x01 << LED_PIN);  // LED OFF
	}

}


