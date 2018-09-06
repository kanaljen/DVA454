#include "board.h"
#include "compiler.h"

#define CYCLES_PER_MS (115/11)
#define GPIO_MAX_PIN_NUMBER 32
#define LED0_PORT ( LED0_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED0_PIN ( LED0_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED0_BIT_VALUE (1 << LED0_PIN )

#define LED1_PORT ( LED1_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED1_PIN ( LED1_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED1_BIT_VALUE (1 << LED1_PIN )

#define LED2_PORT ( LED2_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED2_PIN ( LED2_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED2_BIT_VALUE (1 << LED2_PIN )

#define BUTTON_PORT0 (GPIO_PUSH_BUTTON_0 >> 5)
#define BUTTON_PIN0 (1 << (GPIO_PUSH_BUTTON_0 & 0x1f))

#define BUTTON_PORT1 (GPIO_PUSH_BUTTON_1 >> 5)
#define BUTTON_PIN1 (1 << (GPIO_PUSH_BUTTON_1 & 0x1f))

#define BUTTON_PORT2 (GPIO_PUSH_BUTTON_2 >> 5)
#define BUTTON_PIN2 (1 << (GPIO_PUSH_BUTTON_2 & 0x1f))

void mdelay(int ms){
	long volatile cycles = (ms * CYCLES_PER_MS);
	while (cycles != 0){
		cycles--;
	}
}

void initButton(void){
	
	
	
}

void initLED (void)
{
	volatile avr32_gpio_port_t * led0_port;
	led0_port = &AVR32_GPIO.port[LED0_PORT];
	led0_port->gpers = LED0_BIT_VALUE;
	led0_port->ovrs = LED0_BIT_VALUE;
	led0_port->oders = LED0_BIT_VALUE;
	
	volatile avr32_gpio_port_t * led1_port;
	led1_port = &AVR32_GPIO.port[LED1_PORT];
	led1_port->gpers = LED1_BIT_VALUE;
	led1_port->ovrs = LED1_BIT_VALUE;
	led1_port->oders = LED1_BIT_VALUE;
	
	volatile avr32_gpio_port_t * led2_port;
	led2_port = &AVR32_GPIO.port[LED2_PORT];
	led2_port->gpers = LED2_BIT_VALUE;
	led2_port->ovrs = LED2_BIT_VALUE;
	led2_port->oders = LED2_BIT_VALUE;
}
int main (void)
{
	
	int i;
	
	initLED ();
	
	//Init button port
	volatile int button_state0;
	volatile avr32_gpio_port_t * button_port0;
	button_port0 = &AVR32_GPIO.port[BUTTON_PORT0];
	
	volatile int button_state1;
	volatile avr32_gpio_port_t * button_port1;
	button_port0 = &AVR32_GPIO.port[BUTTON_PORT1];
	
	volatile int button_state2;
	volatile avr32_gpio_port_t * button_port2;
	button_port0 = &AVR32_GPIO.port[BUTTON_PORT2];
	
	button_port0 = &AVR32_GPIO.port[BUTTON_PORT0];
	button_port0->gpers = BUTTON_PIN0;
	button_port0->oderc = BUTTON_PIN0;
	button_port1 = &AVR32_GPIO.port[BUTTON_PORT1];
	button_port1->gpers = BUTTON_PIN1;
	button_port1->oderc = BUTTON_PIN1;
	button_port2 = &AVR32_GPIO.port[BUTTON_PORT2];
	button_port2->gpers = BUTTON_PIN2;
	button_port2->oderc = BUTTON_PIN2;
	
	while(1) {
		button_state0 = button_port0->pvr & BUTTON_PIN0;
		button_state1 = button_port1->pvr & BUTTON_PIN1;
		button_state2 = button_port2->pvr & BUTTON_PIN2;
		
		//Event 1
		if(!button_state0){
			for(i = 0; i <= 10; i++){
				AVR32_GPIO.port[LED0_PORT].ovrs = LED0_BIT_VALUE;
				mdelay(300);
				AVR32_GPIO.port[LED0_PORT].ovrc = LED0_BIT_VALUE;
				mdelay(300);
			}
		}
		//Event 2
		if(button_state1){
			AVR32_GPIO.port[LED1_PORT].ovrs = LED1_BIT_VALUE;
		}
		if(!button_state1){
			AVR32_GPIO.port[LED1_PORT].ovrc = LED1_BIT_VALUE;
		}
		
		//Event 3
		if(!button_state2){
			AVR32_GPIO.port[LED2_PORT].ovrt = LED2_BIT_VALUE;
			while(!button_state2){
				button_state2 = button_port2->pvr & BUTTON_PIN2;
			}
		}
		
	}
	while(1);
}