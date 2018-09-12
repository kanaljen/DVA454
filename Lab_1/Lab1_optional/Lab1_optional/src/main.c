#include "board.h"
#include "compiler.h"

//Define clock
#define CYCLES_PER_MS (115/11)

//Define number of pins
#define GPIO_MAX_PIN_NUMBER 32

//Define LED 0
#define LED0_PORT ( LED0_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED0_PIN ( LED0_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED0_BIT_VALUE (1 << LED0_PIN )

//Define LED 1
#define LED1_PORT ( LED1_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED1_PIN ( LED1_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED1_BIT_VALUE (1 << LED1_PIN )

//Define LED 2
#define LED2_PORT ( LED2_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED2_PIN ( LED2_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED2_BIT_VALUE (1 << LED2_PIN )

//Define LED 3
#define LED3_PORT ( LED3_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED3_PIN ( LED3_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED3_BIT_VALUE (1 << LED3_PIN )

//Define button 0
#define BUTTON_PORT0 (GPIO_PUSH_BUTTON_0 >> 5)
#define BUTTON_PIN0 (1 << (GPIO_PUSH_BUTTON_0 & 0x1f))


//Initialize clock
void mdelay(int ms){
	long volatile cycles = (ms * CYCLES_PER_MS);
	while (cycles != 0){
		cycles--;
	}
}


void initLED (void)
{
	volatile avr32_gpio_port_t * led0_port; //Initialize LED 0 port
	led0_port = &AVR32_GPIO.port[LED0_PORT]; //Map LED port
	led0_port->gpers = LED0_BIT_VALUE; //Set Enable register
	led0_port->ovrs = LED0_BIT_VALUE; //Set output value register
	led0_port->oders = LED0_BIT_VALUE; //Set output drive register
	
	volatile avr32_gpio_port_t * led1_port; //Initialize LED 1 port
	led1_port = &AVR32_GPIO.port[LED1_PORT];  //Set Enable register
	led1_port->gpers = LED1_BIT_VALUE; //Set Enable register
	led1_port->ovrs = LED1_BIT_VALUE; //Set output value register
	led1_port->oders = LED1_BIT_VALUE; //Set output drive register
	
	volatile avr32_gpio_port_t * led2_port; //Initialize LED 2 port
	led2_port = &AVR32_GPIO.port[LED2_PORT]; //Set Enable register
	led2_port->gpers = LED2_BIT_VALUE; //Set Enable register
	led2_port->ovrs = LED2_BIT_VALUE; //Set output value register
	led2_port->oders = LED2_BIT_VALUE; //Set output drive register
	
	volatile avr32_gpio_port_t * led3_port; //Initialize LED 3 port
	led3_port = &AVR32_GPIO.port[LED3_PORT]; //Set Enable register
	led3_port->gpers = LED3_BIT_VALUE; //Set Enable register
	led3_port->ovrs = LED3_BIT_VALUE; //Set output value register
	led3_port->oders = LED3_BIT_VALUE; //Set output drive register	
}

int main (void)
{
	initLED (); //Initialize LED function
	//initButton();

	volatile int button_state0; //Initialize button state 0
	
	while(1) {
		button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0; //Read Button 0

		if(!button_state0){
			
			//Forwards
			AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE; //Toggle LED0
			mdelay(500);
			AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE; //Toggle LED0
			AVR32_GPIO.port[LED1_PORT].ovrt = LED1_BIT_VALUE; //Toggle LED1
			mdelay(500);
			AVR32_GPIO.port[LED1_PORT].ovrt = LED1_BIT_VALUE; //Toggle LED1
			AVR32_GPIO.port[LED2_PORT].ovrt = LED2_BIT_VALUE; //Toggle LED2
			mdelay(500);
			AVR32_GPIO.port[LED2_PORT].ovrt = LED2_BIT_VALUE; //Toggle LED2
			AVR32_GPIO.port[LED3_PORT].ovrt = LED3_BIT_VALUE; //Toggle LED3		
			mdelay(500);
			
			//Backwards
			AVR32_GPIO.port[LED3_PORT].ovrt = LED3_BIT_VALUE; //Toggle LED3
			AVR32_GPIO.port[LED2_PORT].ovrt = LED2_BIT_VALUE; //Toggle LED2
			mdelay(500);
			AVR32_GPIO.port[LED2_PORT].ovrt = LED2_BIT_VALUE; //Toggle LED2
			AVR32_GPIO.port[LED1_PORT].ovrt = LED1_BIT_VALUE; //Toggle LED1
			mdelay(500);
			AVR32_GPIO.port[LED1_PORT].ovrt = LED1_BIT_VALUE; //Toggle LED1
			AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE; //Toggle LED0
			mdelay(500);
			AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE; //Toggle LED0
		}
		
		
	}
	while(1);
}