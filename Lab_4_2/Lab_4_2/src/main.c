#include "functions.h"
#include "defines.h"

int main (void)
{
	LED0_init();
	CLK_init();
	TC_init();
	
	int RC_MAX = 1500;
	float duty_cycle = 0.75;
	
	TC_RC = RC_MAX;
	while(1)
	{
		if(TC_CV <= RC_MAX * duty_cycle)
			LED0_output(0);
			
		if(TC_CV > RC_MAX * duty_cycle)
			LED0_output(1);
	}
	
}

