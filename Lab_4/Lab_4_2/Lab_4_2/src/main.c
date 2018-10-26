#include "stdio.h"
#include "led.h"
#include "tc.h"

int main (void)
{
	LED_init();
	TC_init();
	float duty_cycle = 0.3;

	while(TRUE){
		if (TC_value()>(RC_VALUE*(1-duty_cycle)))LED_set(TRUE);
		else LED_set(FALSE); 
	}
}
