#include "functions.h"

#define reset 0x00

void USART_reset(volatile avr32_usart_t *usart)
{
	usart->CR.rstrx = 1;   //Resets the receiver (1)
	usart->CR.rsttx = 1;   //Resets the transmitter (1)
	usart->CR.rststa = 1;  //Resets status bits (1)
	usart->CR.rstnack = 1; //Reset non acknowledge
	usart->CR.rxdis = 1;   //Disable receiver
	usart->CR.txdis = 1;   //Disable transmitter
}

void USART_init(volatile avr32_usart_t * usart)
{
	
	//Reset (Set all bits to 0 in the used registers)
	USART_reset(usart); 
	//Control Register
	usart->CR.rxdis = 0;   //Should be 0 to NOT disable receiver (0)
	usart->CR.rxen =  1;   //Enables receiver (1)
	usart->CR.txdis = 0;   //Should be 0 to NOT disable transmitter (0)
	usart->CR.txen = 1;    //Enables the transmitter (1)
	usart->CR.sttbrk = 0;  //Start Break UNSURE
	usart->CR.stpbrk = 0;  //Stop Break UNSURE
	usart->CR.sttto = 0;   //Start time-out UNSURE
	usart->CR.senda = 0;   //Send address (In Multidrop mode only) (0)
	usart->CR.rstit = 0;   //Reset Iterations (ISO7816 must be enabled) (0)
	usart->CR.retto = 0;   //Restart time-out (0)
	usart->CR.rtsen = 0;   //Request to send Enable UNSURE
	usart->CR.rtsdis = 0;  //Request to send Disable UNSURE
	
	//Mode register MR
	usart->MR.mode = 0;          //Mode, normal 0000
	usart->MR.usclks = 0;        //Clock selection	
	usart->MR.chrl = 3;          //Character length
	usart->MR.sync = 0;          //Synchronous mode select
	usart->MR.par = 0;           //Parity type
	usart->MR.nbstop = 0;        //Number of stop bits
	usart->MR.chmode = 0;        //Channel mode
	usart->MR.msbf = 0;          //Bit order
	usart->MR.mode9 = 0;         //9-bit character length
	usart->MR.clko = 1;          //Clock output select
	usart->MR.over = 0;          //Oversampling mode
	usart->MR.inack = 0;         //Inhibit non acknowledge
	usart->MR.dsnack = 0;        //Disable successive NACK
	usart->MR.var_sync = 0;      //Variable synchronization of Command
	usart->MR.max_iteration = 0; //Max iterations in ISO7816 mode
	usart->MR.filter = 0;        //Infrared receiver line filter 
	usart->MR.man = 0;           //Manchester encoder enable
	usart->MR.modsync = 0;       //Manchester synchronization mode
	usart->MR.onebit = 0;        //Start frame delimiter selector
	
	//Baud Rate Generator Register
	usart->BRGR.fp = 0;    //Fractional divider 0, 1-7
	usart->BRGR.cd = 9600; //Baudrate? UNSURE BOUT DIISSS
	
	//Transmit Holding Register
	usart->THR.txsynh = 0; //The next char sent is encoded as a data (DATA SYNC)
	
}