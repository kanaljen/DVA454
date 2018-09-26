#include "functions.h"
#include "defines.h"

int main (void)
{
	LED0_init();
	CLK_init();
	TC_init();
	
	
	/* The RC compare value is set to 1500 as it is 1/1000 of the clock frequency (1500 kHz) */
	int RC_compare = 1500;
	float duty_cycle = 0.75;
	
	TC_RC = RC_compare;
	/* Each cycle is 1 ms (1 kHz) */
	while(1)
	{
		/* When counter value is lower than RC compare value times the duty cycle, the LED is ON */
		if(TC_CV <= RC_compare * duty_cycle) 
			LED0_output(1);
			
		/* When counter value is higher than RC compare value times the duty cycle, the LED is OFF */	
		if(TC_CV > RC_compare * duty_cycle)
			LED0_output(0);
	}
	
}

