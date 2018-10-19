#ifndef TIMEC_H_
#define TIMEC_H_

#include "compiler.h"
#include "board.h"
#include "tc.h"
#include "intc.h"
#include "gpio.h"
#include <stdio.h>

#define TRUE 1
#define FALSE 0

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

void TC_init(volatile avr32_tc_t * tc, int tc_channel, __int_handler handler);
void USART_init(volatile avr32_usart_t * usart);
char USART_pollChar();
void USART_putStr(char *c);
void USART_reset(volatile avr32_usart_t * usart);

#endif /* FUNCTIONS_H_ */