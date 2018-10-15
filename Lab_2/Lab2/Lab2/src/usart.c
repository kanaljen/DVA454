#include "usart.h"

void USART_init(volatile avr32_usart_t * usart)
{
	// Reset USART
	USART_reset(usart);
	
	volatile avr32_pm_t *PM = &AVR32_PM;
	// Oscillator 0 Control Register
	PM->OSCCTRL0.mode = 4;				// Crystal is connected to XIN/XOUT - Oscillator is used with gain G0 ( XIN from 0.4 MHz to 0.9 MHz ).
	PM->OSCCTRL0.startup = 6;			// Startup Time 142 ms
	
	// Main Clock Control
	PM->MCCTRL.osc0en = 1;				// Oscillator 0 ENABLED
	PM->MCCTRL.mcsel = 1;				// Oscillator 0 is the source for the main clock	

	
	// Mode Register
	usart->MR.usclks = 0;        // CLK will be used as the input clock source for Baud Rate Generator
	usart->MR.mode9 = 0;         // Disable 9-bit Character Length,chrl is used to set bits
	usart->MR.chrl = 3;          // Character Length
	usart->MR.sync = 0;          // Synchronous Mode Select, set to async
	usart->MR.par = 4;           // No parity
	usart->MR.nbstop = 0;        // Number of Stop Bits
	usart->MR.chmode = 0;        // Channel Mode set to NORMAL
	usart->MR.over = 0;          // Oversampling 8x
	
	// Calculate Clock Divider
	usart->BRGR.fp = 0;                 // No fraction
	int CD = 12000000/(8*9600*(2-0));   // Calculate
	usart->BRGR.cd = CD; 
	 
	// Prepare Holding Register
	usart->THR.txsynh = 0;   //The next char sent is encoded as a data (DATA SYNC)
	
	// Get GPIO address
	volatile avr32_gpio_port_t *gpio = &AVR32_GPIO.port[0]; 
	
	// Select function for RXD

	gpio->pmr0c = 1 << USART_RXD_PIN;	// Peripheral MUX register
	gpio->pmr1c = 1 << USART_RXD_PIN;	// 00 set USART1 - RXD
	gpio->gperc = 1 << USART_RXD_PIN;	// GPIO Enable Register
	
	// Select function for TXD
	gpio->pmr0c = 1 << USART_TXD_PIN;	// Peripheral MUX register
	gpio->pmr1c = 1 << USART_TXD_PIN;	// 00 set USART1 - TXD
	gpio->gperc = 1 << USART_TXD_PIN;	// GPIO Enable Register
	
}

char USART_getChar()
{
	volatile avr32_usart_t *usart = &AVR32_USART1;
	char RX_HOLD;
	
	while(!usart->CSR.rxrdy);	// Receive Holding Register Ready
	if(!usart->RHR.rxsynh)		// RXSYNH: Received Sync, 0 = Last Character received is a Data.
	{
		RX_HOLD = usart->RHR.rxchr; 
	}
	return RX_HOLD;
}
void USART_putChar(char c)
{
	volatile avr32_usart_t *usart = &AVR32_USART1;
	while(!usart->CSR.txrdy);	// Transmitter Ready
	usart->THR.txchr = c;		// Next character to be transmitted after the current character if TXRDY is not set.
}
void USART_reset(volatile avr32_usart_t *usart)
{
	// Enable RESET
	usart->CR.rstrx = 1;   // Receiver
	usart->CR.rsttx = 1;   // Transmitter
	
	// Disable RESET
	usart->CR.rstrx = 0;   // Receiver
	usart->CR.rsttx = 0;   // Transmitter

	// Enable Receiver/Transmitter
	usart->CR.rxdis = 0;   // Do NOT disable Receiver
	usart->CR.rxen =  1;   // DO enable Receiver
	usart->CR.txdis = 0;   // Do NOT disable Transmitter
	usart->CR.txen = 1;    // DO enable Transmitter

}
