#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "compiler.h"

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

//Define button 0
#define BUTTON_PORT0 (GPIO_PUSH_BUTTON_0 >> 5)
#define BUTTON_PIN0 (1 << (GPIO_PUSH_BUTTON_0 & 0x1f))

//Define button 1
#define BUTTON_PORT1 (GPIO_PUSH_BUTTON_1 >> 5)
#define BUTTON_PIN1 (1 << (GPIO_PUSH_BUTTON_1 & 0x1f))

//Define button 1
#define BUTTON_PORT2 (GPIO_PUSH_BUTTON_2 >> 5)
#define BUTTON_PIN2 (1 << (GPIO_PUSH_BUTTON_2 & 0x1f))

void init_LED(void)
{
	volatile avr32_gpio_port_t * led0_port = &AVR32_GPIO.port[LED0_PORT];
	led0_port->gpers = LED0_BIT_VALUE; //Set Enable register
	led0_port->ovrs = LED0_BIT_VALUE; //Set output value register
	led0_port->oders = LED0_BIT_VALUE; //Set output drive register
	
	volatile avr32_gpio_port_t * led1_port = &AVR32_GPIO.port[LED1_PORT];
	led1_port->gpers = LED1_BIT_VALUE; //Set Enable register
	led1_port->ovrs = LED1_BIT_VALUE; //Set output value register
	led1_port->oders = LED1_BIT_VALUE; //Set output drive register
	
	volatile avr32_gpio_port_t * led2_port = &AVR32_GPIO.port[LED2_PORT];
	led1_port->gpers = LED2_BIT_VALUE; //Set Enable register
	led1_port->ovrs = LED2_BIT_VALUE; //Set output value register
	led1_port->oders = LED2_BIT_VALUE; //Set output drive register
}
static portTASK_FUNCTION(vLED_TASK1, pvParameters)
{
	volatile avr32_gpio_port_t * led0 = &AVR32_GPIO.port[LED0_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 20;
	xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		led0->ovrt = LED0_BIT_VALUE;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		led0->ovrt = LED0_BIT_VALUE;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
static portTASK_FUNCTION(vLED_TASK2, pvParameters)
{
	
	(void) pvParameters;
	
	volatile avr32_gpio_port_t * led1 = &AVR32_GPIO.port[LED1_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10;
	xLastWakeTime = xTaskGetTickCount();

	while(1)
	{
		led1->ovrt = LED1_BIT_VALUE;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		led1->ovrt = LED1_BIT_VALUE;
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
int main(void)
{
	init_LED();
	xTaskCreate( vLED_TASK1, ( signed char * ) "LEDx", ( unsigned portSHORT ) 256, NULL, (unsigned portBASE_TYPE) 1, ( xTaskHandle * ) NULL );
	xTaskCreate( vLED_TASK2, ( signed char * ) "LEDx", ( unsigned portSHORT ) 256, NULL, (unsigned portBASE_TYPE) 1, ( xTaskHandle * ) NULL );
	vTaskStartScheduler();
	while(1);
	//button0_state = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0; //Read Button 0
	//button1_state = AVR32_GPIO.port[BUTTON_PORT1].pvr & BUTTON_PIN1; //Read Button 1
	//button2_state = AVR32_GPIO.port[BUTTON_PORT2].pvr & BUTTON_PIN2; //Read Button 2
}
