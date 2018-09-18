#include "compiler.h"
#include "board.h"
#include "functions.h"
#include "tc.h"

#define FALSE 0

volatile avr32_tc_t * tc = &AVR32_TC;
int tc_channel = 0;
int value = 14400;
const tc_interrupt_t *bitfield;


// Define the options for waveform generation
static tc_waveform_opt_t waveform_opt =
{
	. channel = tc_channel , // Channel selection .
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


tc_write_rc(tc, tc_channel, value);
tc_init_waveform(tc, waveform_opt);
tc_start(tc, tc_channel); 
INTC_register_interrupt(&irq_handler, AVR32_TC_IRQ0, AVR32_INTC_INT0);
tc_configure_interrupts(tc, tc_channel, bitfield)







tc_stop(tc, tc_channel);