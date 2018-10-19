#include "asf.h"
#include "timec.h"

struct time_struct // Struct to keep time
{
	int hours;
	int minutes;
	int seconds;
	int decisecond;
};

void USART_putNumber(int num);
void send_time(struct time_struct stopwatch);


int main (void)
{
	struct time_struct stopwatch = // Struct to keep time
	{
		.hours = 0,
		.minutes = 0,
		.seconds = 0,
		.decisecond = 0
	};
	
	char c = 'a'; // char for commands
	int mode = 0; // 1 = clock is enabled, 0 = clock is disabled
	
	volatile avr32_usart_t * usart = &AVR32_USART1;
	volatile avr32_tc_t * tc = &AVR32_TC;
	int time_d = 1; // Clock divider, 10 for decisecond

	USART_init(usart);
	TC_init(tc,time_d);
	
	while (TRUE){
		
		// Read USART1
		if (usart->CSR.rxrdy){ // If there is something to read
			c = USART_getChar(); // Get it
		}
		
		// Start stopwatch
		if (c == 's' && mode != 1){
			mode = 1;
			interupts = 0;
		}
		
		// Stop stopwatch
		if (c == 'x' && mode != 0){
			mode = 0;
		}
		
		if (mode == 1){
			int current = interupts;
			stopwatch.hours = current/(3600*time_d); // Edit Hours
			stopwatch.minutes = (current/(60*time_d))%(60); // Edit minutes
			stopwatch.seconds = (current/time_d)%60; // Edit seconds
			stopwatch.decisecond = current%10; //Edit decisecond
		}
		
		send_time(stopwatch);
		
	}


}

void USART_putNumber(int num){
	USART_putChar('0'+(num/10));
	USART_putChar('0'+(num%10));
}

void send_time(struct time_struct stopwatch){
	
	USART_putChar('0' + (interupts/1000000000)%10);
	USART_putChar('0' + (interupts/100000000)%10);
	USART_putChar('0' + (interupts/100000000)%10);
	USART_putChar('0' + (interupts/10000000)%10);
	USART_putChar('0' + (interupts/1000000)%10);
	USART_putChar('0' + (interupts/100000)%10);
	USART_putChar('0' + (interupts/10000)%10);
	USART_putChar('0' + (interupts/1000)%10);
	USART_putChar('0' + (interupts/100)%10);
	USART_putChar('0' + (interupts/10)%10);
	USART_putChar('0' + interupts%10);
	/* USART_putChar('\n');
	USART_putNumber(stopwatch.hours);
	USART_putChar(':');
	USART_putNumber(stopwatch.minutes);
	USART_putChar(':');
	USART_putNumber(stopwatch.seconds);
	USART_putChar('.');
	USART_putChar('0'+(stopwatch.decisecond));
	*/
	USART_putChar('\r');
}


