#include "compiler.h"
#include "board.h"
#include "headers.h"
#include "functions.c"
#include "avr32\usart_400.h"
#include "avr32\uc3a0512.h"

int main(void)
{
	volatile avr32_usart_t *usart = &AVR32_USART1;
	USART_init(usart);
	return 0;
}