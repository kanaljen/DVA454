#include "compiler.h"
#include "board.h"
#include "functions.h"

int main(void)
{
	volatile avr32_usart_t *usart = &AVR32_USART1;
	USART_init(usart);
	return 0;
}