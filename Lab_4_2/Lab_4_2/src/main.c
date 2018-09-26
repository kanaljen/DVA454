#include "functions.h"
#include "defines.h"

int main (void)
{
	LED0_init();
	ext_clk_init();
	TC_init();
	
	int tmp;
	int tmp2;
	int flag = 0;
	int max_cv = 100;
	float duty_cycle = 0;
	int on_cycle = max_cv * duty_cycle;
	int off_cycle = max_cv * (1 - duty_cycle);
	int lumen = 0;
	int lumen_value = 10000;
	
	TC_RC = max_cv;
	while(1)
	{
		tmp = TC_CV;
		on_cycle = max_cv * duty_cycle;
	    off_cycle = max_cv * (1 - duty_cycle);
		
		if (tmp >= off_cycle && !flag)
		{
			LED0_output(0);
			flag = 1;
			TC_CCR = 0x05;
		} 
		else if(tmp >= on_cycle && flag)
		{
			LED0_output(1);
			flag = 0;
			TC_CCR = 0x05;
		}
		if(lumen >= lumen_value)
		{
			duty_cycle = duty_cycle + 0.1;
			lumen_value = lumen_value + 10000;
		}
		if(lumen >= 100000)
		{
			lumen = 0;
			lumen_value = 10000;
			duty_cycle = 0;
		}
			
		lumen++;
		
	}


	
}

