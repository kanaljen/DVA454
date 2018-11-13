#include "TC_driver.h"
#include "USART_driver.h"

int inter = 0;

// Interrupt function. Counts a global variable once in every interrupt (Every 0.01 second)
__attribute__((__interrupt__))static void tc_irq_handler(void)
{
	inter++;
	
	USART_putInt(inter);

	// Clear the interrupt flag
	tc_read_sr(&AVR32_TC, TC_CHANNEL);
}

void tc_init(volatile avr32_tc_t *tc)
{
	// Struct with waveform options for the TC
	static const tc_waveform_opt_t waveform_opt = {
		// Select channel
		.channel  = TC_CHANNEL,
		// Selects type of timer and interrupt
		.wavsel   = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER,
		// External event trigger enable.
		.enetrg   = false,
		// External event selection.
		.eevt     = 0,
		// External event edge selection.
		.eevtedg  = TC_SEL_NO_EDGE,
		// Counter disable when RC compare.
		.cpcdis   = false,
		// Counter clock stopped with RC compare.
		.cpcstop  = false,
		// Burst signal selection.
		.burst    = false,
		// Clock inversion.
		.clki     = false,
		// Internal source clock 3, connected to fPBA / 8.
		.tcclks   = TC_CLOCK_SOURCE_TC3
	};

	// Struct containing interrupt options
	static const tc_interrupt_t tc_interrupt = {
		.cpcs  = 1, // Enable interrupt on RC compare alone
	};
	// Initialize the timer/counter.
	tc_init_waveform(tc, &waveform_opt);
	// Set trigger to every 10ms, Rc = (115200/8) / 100 = 144
	tc_write_rc(tc, TC_CHANNEL, (115200 / 8 / 100));
	// Configure the timer interrupt
	tc_configure_interrupts(tc, TC_CHANNEL, &tc_interrupt);
	// Start the timer/counter.
	tc_start(tc, TC_CHANNEL);
	//Disable the interrupts
	Disable_global_interrupt();
	//Register the RTC interrupt handler to the interrupt controller.
	INTC_init_interrupts();
	INTC_register_interrupt(&tc_irq_handler, AVR32_TC_IRQ0, AVR32_INTC_INT0);
}