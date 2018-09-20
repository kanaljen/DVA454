#include "functions.h"

volatile avr32_tc_t * tc = &AVR32_TC;
volatile avr32_usart_t * usart = &AVR32_USART1;

int tc_channel = 0;
int value = 14400;
volatile int tick = 0;

__attribute__((__interrupt__)) static void tc_irq_handler(void)
{
	tick++;
	tc_read_sr(tc, tc_channel);
}

int main(void)
{

USART_init(usart);	
TC_init(tc, tc_channel, &tc_irq_handler);tc_write_rc(tc, tc_channel, value); //Set a RC compare at 14400 (1 sec)int seconds = 0;int minutes = 0;int hours = 0;char buffer[16];char command;tc_start(tc, tc_channel);while(1){			command = USART_pollChar();	switch (command)
	{
		case 'a':
			tc_start(tc, tc_channel);
			break;
		case 's':
			tc_stop(tc, tc_channel);
			break;
		case 'd':
			tc_start(tc, tc_channel);
// 			sprintf('\033[2J');
// 			USART_putStr(buffer);
			seconds = 0;			minutes = 0;			hours = 0;
			break;
		default:
			break; 
	}	if(tick == 100)	{		if(seconds >= 60)		{			seconds = 0;			minutes++;		}		if(minutes >= 60)		{			minutes = 0;			hours++;		}		sprintf(buffer, "\033[2J\033c%d : %d : %d", hours, minutes, seconds);		USART_putStr(buffer);		seconds++;		tick = 0;	}}return 0;}