#include "compiler.h"
#include "board.h"
#include "functions.h"

//Define number of pins
#define USART_MAX_PIN_NUMBER 31

#define USART_PORT ( USART_RXD_PIN  / USART_MAX_PIN_NUMBER )
#define RX_PIN USART_RXD_PIN
#define RX_BIT_VALUE (1 << RX_PIN )

#define USART_PORT ( USART_TXD_PIN  / USART_MAX_PIN_NUMBER )
#define TX_PIN USART_TXD_PIN
#define TX_BIT_VALUE (1 << TX_PIN )

int main(void)
{
	volatile avr32_usart_t *usart = &AVR32_USART1;
	USART_init(usart);
	return 0;
}