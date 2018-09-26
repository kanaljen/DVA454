#include "functions.h"
#include "defines.h"


void LED0_init(void) 
{
	/* Select the pin we are going to use as GPIO */
	GPIO_PORT1_GPERS = (1 << 27);
	/* Set the value of that pin to HIGH */
	GPIO_PORT1_OVRS = (1 << 27);
	/* Enable the corresponding driver for that pin */
	GPIO_PORT1_ODERS = (1 << 27);
	
}

void LED0_output(uint8_t value)
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

void CLK_init(void)
{
	/* Oscillator 0 is the source for the main clock, it initialized in PM with startup time 142 ms */
	PM_MCCTRL = 0x05;
	PM_OSCCTRL0 = 0x604;
}
void TC_init(void)
{
	/* Waveform options, fPBA / 8 is the clock frequency */
	
	TC_CCR = 0x05;
	TC_CMR = 0x4002;
}
