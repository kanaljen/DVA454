#include "functions.h"
#include <stdio.h>

#define CYCLES_PER_MS (1091) //12000000/(1000*11)
#define releaseA 500
#define releaseB 500
#define releaseC 0
#define taskTime 4000
#define deadlineA 12000	
#define deadlineB 13000	
#define deadlineC 14000
#define lastDeadline (deadlineC+1000)

//Active delay
void mdelay(int ms){
	long volatile cycles = (ms * CYCLES_PER_MS);
	while (cycles != 0){
		cycles--;
	}
}


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
	portTickType StartTime, EndTime, xLastWakeTime, deadline, reset;
	const portTickType tLimit = deadlineA;
	const portTickType release = releaseA;
	const portTickType resetA = lastDeadline;
	xSemaphoreHandle xSem = *(xSemaphoreHandle*)pvParameters;


	while(1)
	{
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, release); //Release delay
		
		StartTime = xTaskGetTickCount(); //Start time for task
		deadline = StartTime+tLimit; //Get deadline for current cycle
		reset = StartTime+resetA; //Get the reset for current cycle
		
		usart_write_line(configDBG_USART, "Start - Task A\n");
		if(xSemaphoreTake(xSem, deadlineA))
		{
			usart_write_line(configDBG_USART , "Task A - Semaphore Take\n");
			
			led0->ovrc = LED0_BIT_VALUE; //Turn on LED 0
			mdelay(taskTime); //Active delay
			led0->ovrs = LED0_BIT_VALUE; //Turn off LED 0
		}
			
		EndTime = xTaskGetTickCount(); //See when task finishes
		if(deadline < EndTime)
			usart_write_line(configDBG_USART, "DEADLINE MISS - Task A\n");
		else
			usart_write_line(configDBG_USART, "Success - Task A\n");
			
		usart_write_line(configDBG_USART , "Task A - Semaphore Give\n");
		xSemaphoreGive(xSem);
		
		EndTime = xTaskGetTickCount(); //Update EndTime for last delay
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, (resetA - (EndTime - StartTime))); //Delay until next cycle

	}
}
void vLED_TASK1(void* pvParameters)
{
	volatile avr32_gpio_port_t * led1 = &AVR32_GPIO.port[LED1_PORT];
	portTickType StartTime, EndTime, xLastWakeTime, deadline, reset;
	const portTickType tLimit = deadlineB;
	const portTickType release = releaseB;
	const portTickType resetB = lastDeadline;
	xSemaphoreHandle xSem = *(xSemaphoreHandle*)pvParameters;

	while(1)
	{
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, release); //Release delay
		
		StartTime = xTaskGetTickCount(); //Start time for task
		deadline = StartTime+tLimit; //Get deadline for current cycle
		reset = StartTime+resetB; //Get the reset for current cycle
		

		usart_write_line(configDBG_USART, "Start - Task B\n");		

		led1->ovrc = LED1_BIT_VALUE; //Turn on LED 1		
		mdelay(taskTime); //Active delay
		led1->ovrs = LED1_BIT_VALUE; //Turn off LED 1

		EndTime = xTaskGetTickCount(); //See when task finishes
		if(deadline < EndTime )
			usart_write_line(configDBG_USART, "DEADLINE MISS - Task A\n");
		else
			usart_write_line(configDBG_USART, "Success - Task B\n");

		EndTime = xTaskGetTickCount(); //Update EndTime for last delay
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, (resetB - (EndTime - StartTime))); //Delay until next cycle
	}
}
void vLED_TASK2(void* pvParameters)
{
	volatile avr32_gpio_port_t * led2 = &AVR32_GPIO.port[LED2_PORT];
	portTickType StartTime, EndTime, xLastWakeTime, deadline, reset;
	const portTickType tLimit = deadlineC;
	const portTickType release = releaseC;
	const portTickType resetC = lastDeadline;	
	xSemaphoreHandle xSem = *(xSemaphoreHandle*)pvParameters;
	
	while(1)
	{
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, release); //Release delay	
		
		StartTime = xTaskGetTickCount(); //Start time for task
		deadline = StartTime+tLimit; //Get deadline for current cycle
		reset = StartTime+resetC; //Get the reset for current cycle
	
		usart_write_line(configDBG_USART, "Start - Task C\n");	
		
		if(xSemaphoreTake(xSem, deadlineC))
		{
			usart_write_line(configDBG_USART , "Task C - Semaphore Take\n");
			
			led2->ovrc = LED2_BIT_VALUE; //Turn on LED 2
			mdelay(taskTime); //Active delay
			led2->ovrs = LED2_BIT_VALUE; //Turn off LED 2
		}
		
		EndTime = xTaskGetTickCount(); //See when task finishes
		
		if(deadline < EndTime )
			usart_write_line(configDBG_USART, "DEADLINE MISS - Task C\n");
		else
			usart_write_line(configDBG_USART, "Success - Task C\n");	
		
		usart_write_line(configDBG_USART , "Task C - Semaphore Give\n");	
		xSemaphoreGive(xSem);
		
		EndTime = xTaskGetTickCount(); //Update EndTime for last delay
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, (resetC - (EndTime - StartTime))); //Delay until next cycle
	}
}