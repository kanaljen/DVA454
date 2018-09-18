#include "functions.h"

volatile avr32_tc_t * tc = &AVR32_TC;
volatile avr32_usart_t * usart = &AVR32_USART1;
int tc_channel = 0;
int value = 14400;

// Define the options for waveform generation
static tc_waveform_opt_t waveform_opt =
{
	. channel = 0 , // Channel selection .
	. wavsel = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER ,// Waveform selection: Up mode with automatic trigger (reset) on RC compare .
	. enetrg = FALSE , // External event trigger enable.
	. eevt = 0, // External event selection.
	. eevtedg = TC_SEL_NO_EDGE , // External event edge selection.
	. cpcdis = FALSE , // Counter disable when RC compare.
	. cpcstop = FALSE , // Counter clock stopped with RC compare .
	. burst = FALSE , // Burst signal selection .
	. clki = FALSE , // Clock inversion .
	. tcclks = TC_CLOCK_SOURCE_TC3 // Internal clock source 3, connected to fPBA / 8.
};

// Define which timer signals that will cause interrupts
static const tc_interrupt_t TC_INTERRUPT_OPT =
{
	// we interrupt on RC compare match with CV
	. cpcs = 1, // RC compare status
};

__attribute__((__interrupt__)) static void tc_irq_handler(void)
{
	// Before exiting:
	// Clear the interrupt flag, this is done
	// by reading the TC Status Register, SR.
	tc_read_sr(tc, tc_channel);
}

int main(void)
{

USART_init(usart);	
TC_init(tc);
//tc_write_rc(tc, tc_channel, value);
tc_init_waveform(tc, &waveform_opt);

//Interrupt mess
Disable_global_interrupt();
INTC_init_interrupts();
tc_configure_interrupts(tc, tc_channel, &TC_INTERRUPT_OPT);	
INTC_register_interrupt(&tc_irq_handler, AVR32_TC_IRQ0, AVR32_INTC_INT0);
Enable_global_interrupt();
//End of mess

volatile int count;char command;while(1){	command = USART_getChar();	if(command == 's')		tc_start(tc, tc_channel);	else if(command == 'a')		tc_stop(tc, tc_channel);		count = tc_read_tc(tc, tc_channel);	USART_putChar(count);}return 0;}