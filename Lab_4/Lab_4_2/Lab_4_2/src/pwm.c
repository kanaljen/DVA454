/*
 * pwm.c
 *
 * Created: 2018-10-25 16:16:41
 *  Author: sbn14007
 */ 

#include "pwm.h"

void PWM_init(void){
	TC_init();
	PM_init();
}

void TC_init(void)
{
	/* Counter Clock Enabled,  software trigger set (clock is reset and restarted) */
	TC_CCR_ADDRESS = 0x05;  // 0x05 = 101 (CLKEN=1,SWTRG=1), page 657
	/* Select TIMER_CLOCK3, RC Compare resets the counter and starts the counter clock */
	TC_CMR_ADDRESS = 0x4002;  // 0x4002 = 100000000000010 (TCCLKS=010 ,CPCTRG=1), page 658
}

void PM_init(void)
{
	/* Set Oscillator 0 as main clock, Enable Oscillator 0 */
	PM_MCCTRL_ADDRESS = 0x05; // 0x05 = 101 (MCSEL=01,OSC0EN=1), page 68
	/* Crystal is connected to XIN/XOUT - Oscillator is used with gain G0, startup 142 ms */
	PM_OSCCTRL0_ADDRESS = 0x604; // 0x604 = 11000000100 (MODE=100,STARTUP=110), page 74
}
