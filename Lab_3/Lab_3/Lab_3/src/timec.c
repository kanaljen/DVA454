#include "timec.h"

int interupts;

__attribute__((__interrupt__))static void tc_irq_handler(void)
{
	// Count no. interupts
	interupts++;

	// Reset interrupt
	tc_read_sr(&AVR32_TC, TC_CHANNEL);
}

void TC_init(volatile avr32_tc_t * tc, int time_d){
	
	// Set RC value
	tc_write_rc(tc, TC_CHANNEL, (115200/(8/time_d)));
	
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
