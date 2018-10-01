#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "compiler.h"
#include "functions.h"

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
void vLED_TASK0(void* pvParameters)
{
	char c[32] = "LED 1 toggled\n";
	volatile avr32_gpio_port_t * led0 = &AVR32_GPIO.port[LED0_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 1000;

	while(1)
	{
		xLastWakeTime = xTaskGetTickCount();
		led0->ovrt = LED0_BIT_VALUE;
		USART_putStr(c);
		USART_pollChar();
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		led0->ovrt = LED0_BIT_VALUE;
		USART_putStr(c);
		USART_pollChar();
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vLED_TASK1(void* pvParameters)
{
	char c[32] = "LED 1 toggled\n";
	volatile avr32_gpio_port_t * led1 = &AVR32_GPIO.port[LED1_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 2000;

	while(1)
	{
		
		led1->ovrt = LED1_BIT_VALUE;
		USART_putStr(c);
		USART_pollChar();
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		
		led1->ovrt = LED1_BIT_VALUE;
		USART_putStr(c);
		USART_pollChar();
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vLED_TASK2(void* pvParameters)
{
	char c[32] = "LED 2 toggled\n";
	volatile avr32_gpio_port_t * led2 = &AVR32_GPIO.port[LED2_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 3000;

	while(1)
	{
		xLastWakeTime = xTaskGetTickCount();
		led2->ovrt = LED2_BIT_VALUE;
		USART_putStr(c);
		USART_pollChar();
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		led2->ovrt = LED2_BIT_VALUE;
		USART_putStr(c);
		USART_pollChar();
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vbutton_TASK0(void* pvParameters)
{
	char c[32] = "Button 0 pressed\n";
	char c1[32] = "LED 0 toggled\n";
	volatile int button_state0; //Initialize button state 0	
	volatile avr32_gpio_port_t * led0 = &AVR32_GPIO.port[LED0_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10000;
	xTaskHandle task  = *(xTaskHandle *)pvParameters;
	
	while(1){
		
		while(button_state0)
			button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0;
		
		if(!button_state0){
			USART_putStr(c);
			USART_pollChar();
			vTaskSuspend(task);
			led0->ovrc = LED0_BIT_VALUE;
			USART_putStr(c1);
			USART_pollChar();
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency);
			vTaskResume(task);
			
			while(!button_state0)
				button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0; //Read Button 1
		}
	}	
}
void vbutton_TASK1(void* pvParameters)
{
	char c[32] = "Button 1 pressed\n";
	char c1[32] = "LED 1 toggled\n";
	volatile int button_state1; //Initialize button state 1	
	volatile avr32_gpio_port_t * led1 = &AVR32_GPIO.port[LED1_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10000;
	xTaskHandle task  = *(xTaskHandle *)pvParameters;
	
	while(1){
		
		while(button_state1)
			button_state1 = AVR32_GPIO.port[BUTTON_PORT1].pvr & BUTTON_PIN1;
		
		if(!button_state1){
			USART_putStr(c);
			USART_pollChar();
			vTaskSuspend(task);
			led1->ovrc = LED1_BIT_VALUE;
			USART_putStr(c1);
			USART_pollChar();
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency);
			vTaskResume(task);
			
			while(!button_state1)
				button_state1 = AVR32_GPIO.port[BUTTON_PORT1].pvr & BUTTON_PIN1; //Read Button 1
		}
	}	
}
void vbutton_TASK2(void* pvParameters)
{
	char c[32] = "Button 2 pressed\n";
	char c1[32] = "LED 2 toggled\n";
	volatile int button_state2; //Initialize button state 2	
	volatile avr32_gpio_port_t * led2 = &AVR32_GPIO.port[LED2_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10000;
	xTaskHandle task = *(xTaskHandle *)pvParameters;

	while(1){
		
		while(button_state2)
			button_state2 = AVR32_GPIO.port[BUTTON_PORT2].pvr & BUTTON_PIN2; //Read Button 2
		
		if(!button_state2){
			USART_putStr(c);
			USART_pollChar();
			vTaskSuspend(task);
			led2->ovrc = LED2_BIT_VALUE;
			USART_putStr(c1);
			USART_pollChar();
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency);
			vTaskResume(task);
			
			while(!button_state2)
				button_state2 = AVR32_GPIO.port[BUTTON_PORT2].pvr & BUTTON_PIN2; //Read Button 2	
		}
	}	
}
int main(void)
{	
	volatile avr32_usart_t *usart = &AVR32_USART1;
	xTaskHandle *blinkHandle = malloc(sizeof(xTaskHandle *)*3);
	xTaskHandle *buttonHandle = malloc(sizeof(xTaskHandle *)*3);

	USART_init(usart);
	init_LED();
	xTaskCreate(vLED_TASK0, "LED0", 256, NULL, tskIDLE_PRIORITY+1, &(blinkHandle[0]));
	xTaskCreate(vLED_TASK1, "LED1", 256, NULL, tskIDLE_PRIORITY+1, &(blinkHandle[1]));
	xTaskCreate(vLED_TASK2, "LED2", 256, NULL, tskIDLE_PRIORITY+1, &(blinkHandle[2]));
	xTaskCreate(vbutton_TASK0, "BUTTON0", 256, &(blinkHandle[0]), tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(vbutton_TASK1, "BUTTON1", 256, &(blinkHandle[1]), tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(vbutton_TASK2, "BUTTON2", 256, &(blinkHandle[2]), tskIDLE_PRIORITY+1, NULL);
	vTaskStartScheduler();	
	while(1);

}
