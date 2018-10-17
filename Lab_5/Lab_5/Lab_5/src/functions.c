#include "functions.h"
#include <stdio.h>

void USART_init(void)
{
	static const gpio_map_t USART_SERIAL_GPIO_MAP =
	{
		{ serialPORT_USART_RX_PIN , serialPORT_USART_RX_FUNCTION },
		{ serialPORT_USART_TX_PIN , serialPORT_USART_TX_FUNCTION }
	};
	static const gpio_map_t USART_DEBUG_GPIO_MAP =
	{
		{ configDBG_USART_RX_PIN , configDBG_USART_RX_FUNCTION },
		{ configDBG_USART_TX_PIN , configDBG_USART_TX_FUNCTION }
	};
	static const usart_options_t USART_OPTIONS =
	{
		.baudrate = 9600,
		.charlength = 8,
		.paritytype = USART_NO_PARITY ,
		.stopbits = USART_1_STOPBIT ,
		.channelmode = USART_NORMAL_CHMODE
	};
	pm_switch_to_osc0(&AVR32_PM , FOSC0 , OSC0_STARTUP);
	gpio_enable_module(USART_SERIAL_GPIO_MAP , 2);
	gpio_enable_module(USART_DEBUG_GPIO_MAP , 2);
	usart_init_rs232(serialPORT_USART , &USART_OPTIONS , FOSC0);
	usart_init_rs232(configDBG_USART , &USART_OPTIONS , FOSC0);
	
	usart_write_line(configDBG_USART , "USART Initialized\n"); 
	
}
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
	led2_port->gpers = LED2_BIT_VALUE; //Set Enable register
	led2_port->ovrs = LED2_BIT_VALUE; //Set output value register
	led2_port->oders = LED2_BIT_VALUE; //Set output drive register
	
	usart_write_line(configDBG_USART , "LEDs Initialized\n"); 
}
void vLED_TASK0(void* pvParameters)
{
	volatile avr32_gpio_port_t * led0 = &AVR32_GPIO.port[LED0_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 1000; //1 sec

	while(1)
	{	
		taskENTER_CRITICAL(); //Makes sure the message isn't being over written
		usart_write_line(configDBG_USART , "LED 0 toggled\n");
		taskEXIT_CRITICAL();
		
		led0->ovrt = LED0_BIT_VALUE; //Toggles LED0
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency); //Delays 1 sec
	}
}
void vLED_TASK1(void* pvParameters)
{
	volatile avr32_gpio_port_t * led1 = &AVR32_GPIO.port[LED1_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 2000; //2 sec

	while(1)
	{
		taskENTER_CRITICAL(); //Makes sure the message isn't being over written
		usart_write_line(configDBG_USART , "LED 1 toggled\n");
		taskEXIT_CRITICAL();
		
		led1->ovrt = LED1_BIT_VALUE; //Toggles LED1
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency); //Delays 2 sec
	}
}
void vLED_TASK2(void* pvParameters)
{
	volatile avr32_gpio_port_t * led2 = &AVR32_GPIO.port[LED2_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 3000; //3 sec

	while(1)
	{
		taskENTER_CRITICAL(); //Makes sure the message isn't being over written
		usart_write_line(configDBG_USART , "LED 2 toggled\n");
		taskEXIT_CRITICAL();
		
		led2->ovrt = LED2_BIT_VALUE; //Toggles LED2
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency); //Delays 3 sec
	}
}
void vbutton_TASK0(void* pvParameters)
{
	volatile int button_state0; //Initialize button state 0
	volatile avr32_gpio_port_t * led0 = &AVR32_GPIO.port[LED0_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10000; //10 sec
	xTaskHandle task  = *(xTaskHandle *)pvParameters; //Blink LED handle
	
	while(1){
		
		while(button_state0) //While button isn't pushed
			button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0; //Read button
		
		if(!button_state0){ //If button is pushed
			taskENTER_CRITICAL(); //Makes sure the message isn't being over written
			usart_write_line(configDBG_USART , "Button 0 pressed\n");
			taskEXIT_CRITICAL();
			 
			vTaskSuspend(task); //Suspend the task with the same LED with its handle
			
			led0->ovrc = LED0_BIT_VALUE; //Turn on LED
			
			taskENTER_CRITICAL(); //Makes sure the message isn't being over written
			usart_write_line(configDBG_USART , "LED 0 toggled for 10 seconds\n");
			taskEXIT_CRITICAL();
			
			xLastWakeTime = xTaskGetTickCount(); 
			vTaskDelayUntil(&xLastWakeTime, xFrequency); //Delay 10 sec
		
			vTaskResume(task); //Resumes the blinking task
			
			while(!button_state0) //If the button is still pressed after 10 sec, it will not keep running the task
				button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0; //Read Button 0
		}
	}
}
void vbutton_TASK1(void* pvParameters)
{
	volatile int button_state1; //Initialize button state 1
	volatile avr32_gpio_port_t * led1 = &AVR32_GPIO.port[LED1_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10000; //10 sec
	xTaskHandle task  = *(xTaskHandle *)pvParameters; //Blink LED handle
	
	
	while(1){
		
		while(button_state1) //While button isn't pushed
			button_state1 = AVR32_GPIO.port[BUTTON_PORT1].pvr & BUTTON_PIN1; //Read button
		
		if(!button_state1){ //If button is pushed
			
			taskENTER_CRITICAL(); //Makes sure the message isn't being over written
			usart_write_line(configDBG_USART , "Button 1 pressed\n"); 
			taskEXIT_CRITICAL();
			
			vTaskSuspend(task); //Suspend the task with the same LED with its handle
			
			led1->ovrc = LED1_BIT_VALUE; //Turn on LED
			
			taskENTER_CRITICAL(); //Makes sure the message isn't being over written
			usart_write_line(configDBG_USART , "LED 1 toggled for 10 seconds\n"); 
			taskEXIT_CRITICAL();
			
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency); //Delay 10 sec
			
			vTaskResume(task); //Resumes the blinking task
			
			while(!button_state1) //If the button is still pressed after 10 sec, it will not keep running the task
				button_state1 = AVR32_GPIO.port[BUTTON_PORT1].pvr & BUTTON_PIN1; //Read Button 1
		}
	}
}
void vbutton_TASK2(void* pvParameters)
{
	volatile int button_state2; //Initialize button state 2
	volatile avr32_gpio_port_t * led2 = &AVR32_GPIO.port[LED2_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10000; //10 sec
	xTaskHandle task = *(xTaskHandle *)pvParameters; //Blink LED handle
	
	
	while(1){
		
		while(button_state2) //While button isn't pushed
			button_state2 = AVR32_GPIO.port[BUTTON_PORT2].pvr & BUTTON_PIN2; //Read Button 2
		
		if(!button_state2){ //If button is pushed
			
			taskENTER_CRITICAL(); //Makes sure the message isn't being over written
			usart_write_line(configDBG_USART , "Button 2 pressed\n");
			taskEXIT_CRITICAL();
			vTaskSuspend(task); //Suspend the task with the same LED with its handle
			
			led2->ovrc = LED2_BIT_VALUE; //Turn on LED
			
			taskENTER_CRITICAL(); //Makes sure the message isn't being over written
			usart_write_line(configDBG_USART , "LED 2 toggled for 10 seconds\n"); 
			taskEXIT_CRITICAL();
			
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency); //Delay 10 sec
			
			vTaskResume(task); //Resumes the blinking task
			
			while(!button_state2) //If the button is still pressed after 10 sec, it will not keep running the task
				button_state2 = AVR32_GPIO.port[BUTTON_PORT2].pvr & BUTTON_PIN2; //Read Button 2
		}
	}
}