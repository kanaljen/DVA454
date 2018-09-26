#include "functions.h"

volatile avr32_tc_t * tc = &AVR32_TC;
volatile avr32_usart_t * usart = &AVR32_USART1;

int tc_channel = 0;
int value = 15000; //fPBA is 12Mhz which means fPBA / 8 = 1 500 000 and 15000 is a 100th of that.
volatile int tick = 0;

__attribute__((__interrupt__)) static void tc_irq_handler(void) 
{
	tick++; //Every time the RC compare value is reached, the clock ticks. (10ms)
	tc_read_sr(tc, tc_channel); //Resets the compare trigger by reading the SR
}

int main(void)
{

USART_init(usart);	//Initialize USART
TC_init(tc, tc_channel, &tc_irq_handler); //Initialize timer/countertc_write_rc(tc, tc_channel, value); //Set a RC compare at 15000 (10ms)int seconds = 0;int minutes = 0;int hours = 0; char buffer[32]; //Timer stringchar command; //Stop, Start, Reset commandtc_start(tc, tc_channel); //Start timer/counterwhile(1){			command = USART_pollChar(); //Reads if there is any command written via USART	switch (command) //Reads the command
	{
		case 'a': //Starts the clock, this does however reset the CV, might loose max 1 seconds. 
			tc_start(tc, tc_channel);
			break;
		case 's': //Stops the clock
			tc_stop(tc, tc_channel);
			break;
		case 'd': //Resets and starts the clock
			tc_start(tc, tc_channel);
			seconds = 0; //Resets seconds			minutes = 0; //Resets minutes			hours = 0; //Resets hours
			break;
		default: //No command read
			break; 
	}	if(tick == 100) //Once 100 ticks is reached, add 1 second to the clock. A interrupt occurs once every 10 ms	{		if(seconds >= 60)		{			seconds = 0; //Resets seconds			minutes++; //Add a minute		}		if(minutes >= 60)		{			minutes = 0; //Resets minutes			hours++; //Add an hour		}		seconds++; //add a second				sprintf(buffer, "\033[2J\033c%02d : %02d : %02d", hours, minutes, seconds); //Writes to buffer. ANSI reset/clear screen command followed by the graphical timer.		USART_putStr(buffer); //Sends buffer via USART				tick = 0; //Reset ticks from 100	}}return 0;}