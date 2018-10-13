#include "usart.h"

void USART_init(volatile avr32_usart_t * usart)
{
	//Reset
	USART_reset(usart);
	
	// Mode register MR
	usart->MR.usclks = 0;        //Clock selection, USART_CLK selected
	usart->MR.mode9 = 0;         //9-bit character length
	usart->MR.chrl = 3;          //Character length (8 bit)
	usart->MR.sync = 0;          //Synchronous mode select, async selected
	usart->MR.par = 4;           //Parity type (No parity)
	usart->MR.nbstop = 0;        //Number of stop bits (1 stop bit)
	usart->MR.chmode = 0;        //Channel mode (normal)
	usart->MR.over = 0;          //Oversampling mode (16x)
	
	//Baud Rate Generator Register
	usart->BRGR.fp = 0;    // No fraction needed
	int CD = 12000000/(8*9600*(2-0));
	usart->BRGR.cd = CD; 
	
	//Transmit Holding Register
	usart->THR.txsynh = 0;   //The next char sent is encoded as a data (DATA SYNC)
	
	//Enable GPIO ports for communication
	volatile avr32_gpio_port_t *gpio = &AVR32_GPIO.port[0]; 
	
	//Enable RXD pin in GPIO

	gpio->gpers = 1 << USART_RXD_PIN;  //Make the GPIO control the pins
	gpio->pmr0c = 1 << USART_RXD_PIN;  //Select peripheral A (clear) {pmr1, pmr0} = 00
	gpio->pmr1c = 1 << USART_RXD_PIN;  //Select peripheral A (clear)
	gpio->gperc = 1 << USART_RXD_PIN;  //Enable peripheral control
	
	//Enable TXD pin in GPIO

	gpio->gpers = 1 << USART_TXD_PIN;  //Make the GPIO control the pins
	gpio->pmr0c = 1 << USART_TXD_PIN;  //Select peripheral A (clear) {pmr1, pmr0} = 00
	gpio->pmr1c = 1 << USART_TXD_PIN;  //Select peripheral A (clear)
	gpio->gperc = 1 << USART_TXD_PIN;  //Enable peripheral control
	
	//Enable CLK pin in GPIO

	gpio->gpers = 1 << AVR32_USART1_CLK_0_PIN;  //Make the GPIO control the pins
	gpio->pmr0c = 1 << AVR32_USART1_CLK_0_PIN;  //Select peripheral A (clear) {pmr1, pmr0} = 00
	gpio->pmr1c = 1 << AVR32_USART1_CLK_0_PIN;  //Select peripheral A (clear)
	gpio->gperc = 1 << AVR32_USART1_CLK_0_PIN;  //Enable peripheral control
	
	volatile avr32_pm_t *PM = &AVR32_PM; 
	PM->OSCCTRL0.mode = 4;    //Crystal is connected to xin/xout with gain G0
	PM->OSCCTRL0.startup = 6; //Startup time 142 ms
	PM->MCCTRL.osc0en = 1;    //Oscillator 0 ENABLED
	PM->MCCTRL.osc1en = 0;    //Oscillator 1 DISABLED
	PM->MCCTRL.mcsel = 1;     //Oscillator 0 is the source for the main clock	
	
}

char USART_getChar()
{
	volatile avr32_usart_t *usart = &AVR32_USART1;
	char RX_HOLD;
	
	while(!usart->CSR.rxrdy); //rxrdy indicates that a complete character has been received and that rhr is enabled
	if(!usart->RHR.rxsynh) //indicates that the received character is data and not a command
	{
		RX_HOLD = usart->RHR.rxchr; 
	}
	return RX_HOLD;
}
void USART_putChar(char c)
{
	volatile avr32_usart_t *usart = &AVR32_USART1;
	while(!usart->CSR.txrdy); //The transmitter reports to CSR txrdy = 1 which indicates that THR is empty and TXEMPTY is 0
	usart->THR.txchr = c; //Receive whatever data is in the transmit holding register THR
}
void USART_reset(volatile avr32_usart_t *usart)
{
	//Reset
	usart->CR.rstrx = 1;   //Resets the receiver (1)
	usart->CR.rsttx = 1;   //Resets the transmitter (1)
	usart->CR.rststa = 1;  //Resets status bits (1)
	usart->CR.rstnack = 1; //Reset non acknowledge (0)
	
	//"reset" the reset
	usart->CR.rstrx = 0;   //Resets the receiver (0)
	usart->CR.rsttx = 0;   //Resets the transmitter (0)
	usart->CR.rststa = 0;  //Resets status bits (0)
	usart->CR.rstnack = 0; //Reset non acknowledge (0)
	
	//re-enables receiver and transmitter
	usart->CR.rxdis = 0;   //Should be 0 to NOT disable receiver (0)
	usart->CR.rxen =  1;   //Enables receiver (1)
	usart->CR.txdis = 0;   //Should be 0 to NOT disable transmitter (0)
	usart->CR.txen = 1;    //Enables the transmitter (1)

}