/*
 * pwm.c
 *
 * Created: 2018-10-25 16:16:41
 *  Author: sbn14007
 */ 

#include "tc.h"

void TC_init()
{
	TC_CMR_ADDRESS = (1 << 1); // TCCLKS: TIMER_CLOCK3, page 658
	TC_CMR_ADDRESS = (1 << 14); // CPCTRG: RC Compare resets the counter and starts the counter clock, page 659
	TC_CMR_ADDRESS = (1 << 15); // WAVE: Capture Mode is disabled (Waveform Mode is enabled), page 659
	TC_RC_ADDRESS = 14400; // Set RC, page 667
	TC_start(); // Start the clock
}

void TC_start()
{
	TC_CCR_ADDRESS = (1 << 0); // CLKEN: Enables the clock, page 657
	TC_CCR_ADDRESS = (1 << 2); // SWTRG: The counter is reset and the clock is started, page 657
}
