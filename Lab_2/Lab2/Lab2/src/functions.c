#include "functions.h"


#define max_bit_number 32
#define reset 0x00

void USART_reset(volatile avr32_usart_t *usart)
{

}

void USART_init(volatile avr32_usart_t * usart)
{
	
	//Reset (Set all bits to 0 in the used registers)
	USART_reset(usart); 
	//Control Register
	usart->CR.rstrx = 0;   //Resets the receiver (1)
	usart->CR.rsttx = 0;   //Resets the transmitter (1)
	usart->CR.rxen =  0;   //Enables receiver (1)
	usart->CR.rxdis = 0;   //Should be 0 to NOT disable receiver (0)
	usart->CR.txen = 0;    //Enables the transmitter (1)
	usart->CR.txdis = 0;   //Should be 0 to NOT disable transmitter (0)
	usart->CR.rststa = 0;  //Resets status bits (1)
	usart->CR.sttbrk = 0;  //Start Break UNSURE
	usart->CR.stpbrk = 0;  //Stop Break UNSURE
	usart->CR.sttto = 0;   //Start time-out UNSURE
	usart->CR.senda = 0;   //Send Adress (In Multidrop mode only) (0)
	usart->CR.rstit = 0;   //Reset Iterations (ISO7816 must be enabled) (0)
	usart->CR.rstnack = 0; //Reset non acknowledge
	usart->CR.retto = 0;   //Restart time-out (0)
	usart->CR.rtsen = 0;   //Request to send Enable UNSURE
	usart->CR.rtsdis = 0;  //Requste to send Disable UNSURE
	
	//Mode register MR
	usart->MR.mode = 0;
	usart->MR.usclks = 0;
	usart->MR.chmode = 0;
	usart->MR.chrl = 0;
	usart->MR.sync = 0;
	usart->MR.par = 0;
	usart->MR.nbstop = 0;
	usart->MR.chmode = 0;
	usart->MR.msbf = 0;
	usart->MR.mode9 = 0;
	usart->MR.clko = 0;
	usart->MR.over = 0;
	usart->MR.inack = 0;
	usart->MR.dsnack = 0;
	usart->MR.var_sync = 0;
	usart->MR.max_iteration = 0;
	usart->MR.filter = 0;
	usart->MR.man = 0;
	usart->MR.modsync = 0;
	usart->MR.onebit = 0;
	
	//Baud Rate Generator Register
	usart->BRGR.fp = 0; //Fractional divider 0, 1-7
	usart->BRGR.cd = 9600; //Baudrate? UNSURE BOUT DIISSS
	
	//Transmit Holding Register
	usart->THR.txsynh = 0; //The next char sent is encoded as a data (DATA SYNC)
	
}