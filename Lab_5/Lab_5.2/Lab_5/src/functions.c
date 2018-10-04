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
	portTickType StartTime, EndTime;
	const portTickType xFrequency = 4000;
	const portTickType tLimit = 5000;
	xSemaphoreHandle xSem = *(xSemaphoreHandle*)pvParameters;
	int DeadlineMiss = FALSE;

	while(1)
	{
		StartTime = xTaskGetTickCount();
		if(xSemaphoreTake(xSem, tLimit - xFrequency + 1))
		{
			led0->ovrc = LED0_BIT_VALUE;
			usart_write_line(configDBG_USART , "Task A - Semaphore Take\n");
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency);
			led0->ovrs = LED0_BIT_VALUE;
			usart_write_line(configDBG_USART , "Task A - Semaphore Give\n");
			xSemaphoreGive(xSem);
		}
		else
		{
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency - 1);
			DeadlineMiss = TRUE;
		}
		EndTime = xTaskGetTickCount();
		if(DeadlineMiss)
		{
			taskENTER_CRITICAL();
			usart_write_line(configDBG_USART, "DEADLINE MISS - Task A\n");
			taskEXIT_CRITICAL();
			DeadlineMiss = FALSE;
		}
		else
		{
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, tLimit - (EndTime - StartTime));
		}
	}
}
void vLED_TASK1(void* pvParameters)
{
	volatile avr32_gpio_port_t * led1 = &AVR32_GPIO.port[LED1_PORT];
	portTickType xLastWakeTime;
	portTickType StartTime, EndTime;
	const portTickType xFrequency = 4000;
	const portTickType tLimit = 10000;
	xSemaphoreHandle xSem = *(xSemaphoreHandle*)pvParameters;
	int DeadlineMiss = FALSE;

	while(1)
	{
		StartTime = xTaskGetTickCount();
		if(xSemaphoreTake(xSem, tLimit - xFrequency + 1))
		{
			led1->ovrc = LED1_BIT_VALUE;
			usart_write_line(configDBG_USART , "Task B - Semaphore Take\n");
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency);
			led1->ovrs = LED1_BIT_VALUE;
			usart_write_line(configDBG_USART , "Task B - Semaphore Give\n");
			xSemaphoreGive(xSem);			
		}
		else
		{
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency - 1);
			DeadlineMiss = TRUE;
		}
		EndTime = xTaskGetTickCount();
		if(DeadlineMiss)
		{
			taskENTER_CRITICAL();
			usart_write_line(configDBG_USART, "DEADLINE MISS - Task B\n");
			taskEXIT_CRITICAL();
			DeadlineMiss = FALSE;
		}
		else
		{
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, tLimit - (EndTime - StartTime));
		}
	}
}
void vLED_TASK2(void* pvParameters)
{
	volatile avr32_gpio_port_t * led2 = &AVR32_GPIO.port[LED2_PORT];
	portTickType xLastWakeTime;
	portTickType StartTime, EndTime;
	const portTickType xFrequency = 4000;
	const portTickType tLimit = 15000;
	xSemaphoreHandle xSem = *(xSemaphoreHandle*)pvParameters;
	int DeadlineMiss = FALSE;
	
	while(1)
	{
		StartTime = xTaskGetTickCount();
		if(xSemaphoreTake(xSem, tLimit - xFrequency + 1))
		{
			led2->ovrc = LED2_BIT_VALUE;
			usart_write_line(configDBG_USART , "Task C - Semaphore Take\n");
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency);
			led2->ovrs = LED2_BIT_VALUE;
			xSemaphoreGive(xSem);
			usart_write_line(configDBG_USART , "Task C - Semaphore Give\n");
		}
		else
		{
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency - 1);
			DeadlineMiss = TRUE;
		}
		EndTime = xTaskGetTickCount();
		if(DeadlineMiss)
		{
			taskENTER_CRITICAL();
			usart_write_line(configDBG_USART, "DEADLINE MISS - Task C\n");
			taskEXIT_CRITICAL();
			DeadlineMiss = FALSE;
		}
		else
		{
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, tLimit - (EndTime - StartTime));
		}
	}
}