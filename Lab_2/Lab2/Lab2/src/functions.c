#define max_bit_number 32
#define reset 00000000000000000000000000000000

void USART_reset()
{
	volatile avr32_usart_t * usart;
	usart->brgr = reset;
	usart->MR = reset;
	usart->RHR = reset;
	usart->THR = reset;
}

void USART_init(volatile avr32_usart_t * usart)
{
	//reset ?
	USART_reset(); //possibly go into every register we use and set all bits to 0 ?
	
	// set baudrate
	//usart->brgr baud rate generator register
	
	// set message length? 
	// initialize stop bit?
	
	// set mode normal
 	//usart->MR mode register
	
	// enable comms
	//usart->RHR receiver holding register
	//usart->THR transmitter holding register
}