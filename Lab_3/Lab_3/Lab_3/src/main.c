#include <avr32/io.h>
#include "compiler.h"
#include "board.h"
#include "gpio.h"
#include "USART_driver.h"
#include "TC_driver.h"
// #include "stopwatch.h"

int main(void)
{
	volatile avr32_usart_t * usart = USART;
	volatile avr32_tc_t *tc = &AVR32_TC;
	USART_init(usart);						// Init USART
	tc_init(tc);							// Init Timer/Counter

	while(1)
	{
		
	}
	while(1);
}