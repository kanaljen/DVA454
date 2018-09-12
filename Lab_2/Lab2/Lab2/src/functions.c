//#include "headers.h"
#include "avr32\uc3a0512.h"
#include "avr32\usart_400.h"

#define max_bit_number 32
#define reset 0x00

void USART_reset(volatile avr32_usart_t *usart)
{
	//volatile avr32_usart_t *usart = &AVR32_USART1;
	usart->BRGR.cd = 0;
	usart->MR.mode = 0;
	usart->MR.clko = 0;
	usart->MR.nbstop = 0;
	usart->THR.txsynh = 0;
}

void USART_init(volatile avr32_usart_t * usart)
{
	
	//Reset (Set all bits to 0 in the used registers)
	USART_reset(usart); 
	//normal mode
	usart->MR.mode = 0;
	//bit clock initialize
	usart->MR.clko = 1;
	//stop bit, 1 for assync and 2 for sync
	usart->MR.nbstop = 1;
	// set message length? 
	
	//initialize and set baudrate
	usart->BRGR.cd = 9600;
	
	//The next charachter sent is encoded as data (1 for command)
	usart->THR.txsynh = 0;
}