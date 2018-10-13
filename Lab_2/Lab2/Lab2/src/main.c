#include "compiler.h"
#include "board.h"
#include "usart.h"

int main(void)
{
	volatile avr32_usart_t *usart = &AVR32_USART1;
	char x;
	USART_init(usart);
	while(1)
	{
		x = USART_getChar();
		USART_putChar(x);	
	}
	return 0;
}