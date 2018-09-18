#include "functions.h"

void TC_init(volatile avr32_tc_t * tc){
	//Enable GPIO ports for communication
	volatile avr32_gpio_port_t *tc_gpio = &AVR32_GPIO.port[0];

	//Enable TC_CLK0 pin in GPIO
	tc_gpio->gpers = 1; //Make the GPIO control the pins
	tc_gpio->pmr0 = 1;	//Select peripheral A (clear)
	tc_gpio->pmr1 = 0; //Select peripheral A (clear)
	tc_gpio->gperc = 1; //Enable peripheral control
	
	volatile avr32_pm_t *tc_PM = &AVR32_PM;
	tc_PM->OSCCTRL0.mode = 4;    //Crystal is connected to xin/xout with gain G2
	tc_PM->OSCCTRL0.startup = 6; //Startup time 142 ms
	tc_PM->MCCTRL.osc0en = 1;    //Oscillator 0 ENABLED
	tc_PM->MCCTRL.osc1en = 0;    //Oscillator 1 DISABLED
	tc_PM->MCCTRL.mcsel = 1;     //Oscillator 0 is the source for the main clock
}