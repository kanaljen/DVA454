#include "board.h"
#include "compiler.h"

//Define clock
#define CYCLES_PER_MS (115/11)

//Define pins
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

//Define button 0
#define BUTTON_PORT1 (GPIO_PUSH_BUTTON_1 >> 5)
#define BUTTON_PIN1 (1 << (GPIO_PUSH_BUTTON_1 & 0x1f))

//Define button 0
#define BUTTON_PORT2 (GPIO_PUSH_BUTTON_2 >> 5)
#define BUTTON_PIN2 (1 << (GPIO_PUSH_BUTTON_2 & 0x1f))

//Initialize clock
void mdelay(int ms){
	long volatile cycles = (ms * CYCLES_PER_MS);
	while (cycles != 0){
		cycles--;
	}
}

//Initialize buttons
void initButton(void)
{
	AVR32_USART0.port[BUTTON_PORT0].gpers = BUTTON_PIN0;
	AVR32_GPIO.port[BUTTON_PORT0].gpers = BUTTON_PIN0;
	AVR32_GPIO.port[BUTTON_PORT0].oderc = BUTTON_PIN0;
	
	AVR32_GPIO.port[BUTTON_PORT1].gpers = BUTTON_PIN1;
	AVR32_GPIO.port[BUTTON_PORT1].oderc = BUTTON_PIN1;
	
	AVR32_GPIO.port[BUTTON_PORT2].gpers = BUTTON_PIN2;
	AVR32_GPIO.port[BUTTON_PORT2].oderc = BUTTON_PIN2;
}

//Initialize LEDs
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
}
int main (void)
{
	initLED (); //Initialize LED function
	//initButton();

	volatile int button_state0; //Initialize button state 0
	volatile int button_state1; //Initialize button state 1
	volatile int button_state2; //Initialize button state 2
	int count = 0; //Count for event 4

	
	while(1) {
		button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0; //Read Button 0
		button_state1 = AVR32_GPIO.port[BUTTON_PORT1].pvr & BUTTON_PIN1; //Read Button 1
		button_state2 = AVR32_GPIO.port[BUTTON_PORT2].pvr & BUTTON_PIN2; //Read Button 2
		
//------Event 1 - Blinking LED1-----------------------------------------------------------------------------------
		if(!button_state0){
			for(int i = 0; i <= 10; i++){ //Toggle LED 0 10 times.
				AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE; //Toggle LED 0
				mdelay(100);
			}
			AVR32_GPIO.port[LED0_PORT].ovrs = LED0_BIT_VALUE; //Makes sure LED 0 is turned off after the event
		}
		
//------Event 2 - Light LED2 when pushed---------------------------------------------------------------------------
		if(!button_state1){
			AVR32_GPIO.port[LED1_PORT].ovrt = LED1_BIT_VALUE; //Toggle LED 1
			
			count++; //Count for "Event 4"
			
			//Internal loop for when the button is pressed. To not make the led blink. Exits when released
			while(!button_state2){
				button_state2 = AVR32_GPIO.port[BUTTON_PORT2].pvr & BUTTON_PIN2; //Read Button 2
			}
			AVR32_GPIO.port[LED1_PORT].ovrt = LED1_BIT_VALUE; //Toggle LED 1
		}
		
//------Event 3 - Toggle LED3----------------------------------------------------------------------------------------
		if(!button_state2){
			AVR32_GPIO.port[LED2_PORT].ovrt = LED2_BIT_VALUE; //Toggle LED2
			
			//Internal loop for when the button is pressed. To not make the led blink. Exits when released
			while(!button_state2){
				button_state2 = AVR32_GPIO.port[BUTTON_PORT2].pvr & BUTTON_PIN2; //Read Button 2
			}
		}
		
//------Event 4------------------------------------------------------------------------------------------------------
		if(count == 3){ //Starts after Event 2 has run 3 times
			
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
			
			//Backwards
			AVR32_GPIO.port[LED1_PORT].ovrt = LED1_BIT_VALUE; //Toggle LED1
			mdelay(500);
			AVR32_GPIO.port[LED1_PORT].ovrt = LED1_BIT_VALUE; //Toggle LED1
			AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE; //Toggle LED0
			mdelay(500);
			AVR32_GPIO.port[LED0_PORT].ovrt = LED0_BIT_VALUE; //Toggle LED0
			
			count = 0; //Reset Event 4 counter
		}
	}
}