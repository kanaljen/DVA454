#include "timec.h"
#include "usart.h"

int interupts;
struct time_struct stopwatch = // Struct to keep time
{
	.hours = 0,
	.minutes = 0,
	.seconds = 0,
	.decisecond = 0
};
volatile avr32_usart_t * usart = &AVR32_USART1;

__attribute__((__interrupt__))static void tc_irq_handler(void)
{
	char c = 'a';
	int mode = 0;
	
	// Count no. interupts
	interupts++;
	
			/*/ Read USART1
			if (usart->CSR.rxrdy){ // If there is something to read
				c = USART_getChar(); // Get it
			}
			*/
			
			// Start stopwatch
			if (c == 's' && mode != 1){
				mode = 1;
				interupts = 0;
			}
			
			// Stop stopwatch
			if (c == 'x' && mode != 0){
				mode = 0;
			}
			
			int current = interupts;
			stopwatch.hours = current/3600; // Edit Hours
			stopwatch.minutes = (current/60)%60; // Edit minutes
			stopwatch.seconds = current%60; // Edit seconds

	USART_putNumber(interupts);	
	//send_time(stopwatch);
	
	// Reset interrupt
	tc_read_sr(&AVR32_TC, TC_CHANNEL);
	
}

void TC_init(volatile avr32_tc_t * tc){
	
	// Set RC value
	tc_write_rc(tc, TC_CHANNEL, CLK_FRQ);
	
	// Set waveform option
	tc_init_waveform(tc, &waveform_opt);
	
	// Set interrupt to only interrupt on RC
	tc_configure_interrupts(tc, TC_CHANNEL, &tc_interupt_opt);
	
	// Start the TC
	tc_start(tc, TC_CHANNEL);
	
	// Configure interupts
	Disable_global_interrupt();
	INTC_init_interrupts();
	INTC_register_interrupt(&tc_irq_handler, AVR32_TC_IRQ0, AVR32_INTC_INT0);
	Enable_global_interrupt();
	
}

void send_time(struct time_struct stopwatch){
	
	USART_putNumber(stopwatch.hours);
	USART_putChar(':');
	USART_putNumber(stopwatch.minutes);
	USART_putChar(':');
	USART_putNumber(stopwatch.seconds);
	USART_putChar('\r');
	
}

