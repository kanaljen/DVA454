#include "compiler.h"
#include "board.h"
#include "functions.h"

int main(void)
{
	volatile avr32_usart_t *usart = &AVR32_USART1;
	char c;
	USART_init(usart);
	while(1)
	{
		c = USART_getChar();
		USART_putChar(c);	
	}
	return 0;
}