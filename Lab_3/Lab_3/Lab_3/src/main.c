#include "asf.h"
#include "timec.h"
#include "usart.h"

int main (void)
{
	
	volatile avr32_tc_t * tc = &AVR32_TC;
	volatile avr32_usart_t * usart = &AVR32_USART1;
	
	USART_init(usart);
	TC_init(tc);
	
	while (TRUE){}

}


