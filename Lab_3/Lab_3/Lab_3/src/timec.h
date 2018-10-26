#ifndef TIMEC_H_
#define TIMEC_H_

#include "evk1100.h"
#include "tc.h"
#include "intc.h"
#include "interrupt.h"
#include "usart.h"

#define TRUE 1
#define FALSE 0
#define TC_CHANNEL 0
#define CLK_FRQ 14400 // fPBA / 8 (TC_CLOCK_SOURCE_TC3), fPBA = 115200

extern int interupts;			// Global variable

// Define the options for waveform generation
static tc_waveform_opt_t waveform_opt =
{
	. channel = TC_CHANNEL , // Channel selection .
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

struct time_struct // Struct to keep time
{
	int hours;
	int minutes;
	int seconds;
	int decisecond;
};

// Define which timer signals that will cause interrupts
static const tc_interrupt_t tc_interupt_opt =
{
	. cpcs = 1, // Only interrupt on RC
};

// Define functions
__attribute__((__interrupt__))static void tc_irq_handler(void); // Interrupt-handler, add 1 to int interupts each interrupt
void TC_init(volatile avr32_tc_t * tc); // Initiate TC0 with RC set to 115200 Hz/8/time_d
void send_time(struct time_struct stopwatch);

#endif /* FUNCTIONS_H_ */