/*
 * functions.c
 *
 * Created: 2018-10-26 15:16:21
 *  Author: sbn14007
 */ 

#include "functions.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "gpio.h"
#include "pm.h"
#include "board.h"
#include "compiler.h"

const int bport[3] = {BUTTON_PORT0,BUTTON_PORT1,BUTTON_PORT2};
const int bpin[3] = {BUTTON_PIN0,BUTTON_PIN1,BUTTON_PIN2};
const int lport[3] = {LED0_PORT,LED1_PORT,LED2_PORT};
const int lbit[3] = {LED0_BIT_VALUE,LED1_BIT_VALUE,LED2_BIT_VALUE};

void USART_init(void)
{
	static const gpio_map_t USART_SERIAL_GPIO_MAP =
	{
		{ serialPORT_USART_RX_PIN , serialPORT_USART_RX_FUNCTION },
		{ serialPORT_USART_TX_PIN , serialPORT_USART_TX_FUNCTION }
	};
	static const gpio_map_t USART_DEBUG_GPIO_MAP =
	{
		{ configDBG_USART_RX_PIN , configDBG_USART_RX_FUNCTION } ,
		{ configDBG_USART_TX_PIN , configDBG_USART_TX_FUNCTION }
	};
	static const usart_options_t USART_OPTIONS =
	{
		. baudrate = 115200,
		. charlength = 8,
		. paritytype = USART_NO_PARITY ,
		. stopbits = USART_1_STOPBIT ,
		. channelmode = USART_NORMAL_CHMODE
	};
	pm_switch_to_osc0 (& AVR32_PM , FOSC0 , OSC0_STARTUP );
	gpio_enable_module ( USART_SERIAL_GPIO_MAP , 2);
	gpio_enable_module ( USART_DEBUG_GPIO_MAP , 2);
	usart_init_rs232 ( serialPORT_USART , & USART_OPTIONS , FOSC0 );
	usart_init_rs232 ( configDBG_USART , & USART_OPTIONS , FOSC0 );
	
	vWriteLine("USART Initialized\n");
}

void LED_init(void)
{
	for(int i=0;i<3;i++){
		volatile avr32_gpio_port_t * led = &AVR32_GPIO.port[lport[i]];
		led->gpers = lbit[i]; 
		led->ovrs = lbit[i]; 
		led->oders = lbit[i];
	}	
	vWriteLine("All LED:s Initialized\n");
}

void tskBlinking0(void* ptr)
{
	int index = 0;
	portTickType xLastWakeTime;
	const portTickType xMS = 1000;
	xSemaphoreHandle hndSem = *(xSemaphoreHandle*)ptr;

	volatile avr32_gpio_port_t * led = &AVR32_GPIO.port[lport[index]];

	while(TRUE)
	{
		led->ovrt = lbit[index]; 
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xMS); 
	}
}

void tskBlinking1(void* ptr)
{
	int index = 1;
	portTickType xLastWakeTime;
	const portTickType xMS = 2000;

	volatile avr32_gpio_port_t * led = &AVR32_GPIO.port[lport[index]];

	while(TRUE)
	{
		led->ovrt = lbit[index];
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xMS);
	}
}

void tskBlinking2(void* ptr)
{
	int index = 2;
	portTickType xLastWakeTime;
	const portTickType xMS = 3000;

	volatile avr32_gpio_port_t * led = &AVR32_GPIO.port[lport[index]];

	while(TRUE)
	{
		led->ovrt = lbit[index];
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xMS);
	}
}

void tskButtons(void* ptr){
	
	int state[3] = {1,1,1};
	int pressed = FALSE;
	xTaskHandle* task = ptr;
	xTaskHandle* subTask = malloc(sizeof(xTaskHandle*)*3);
	xTaskCreate(tskSubTask,"subTask0",STACK_SIZE,&(task[0]),3,&(subTask[0]));
	xTaskCreate(tskSubTask,"subTask1",STACK_SIZE,&(task[1]),3,&(subTask[1]));
	xTaskCreate(tskSubTask,"subTask2",STACK_SIZE,&(task[2]),3,&(subTask[2]));

	while (TRUE)
	{
		for(int i=0;i<3;i++){
			state[i] = AVR32_GPIO.port[bport[i]].pvr & bpin[i]; 
			if(!state[i] && pressed == FALSE){
				pressed = TRUE;
				vWriteLine("Button pushed!\n");
				while(!state[i])state[i] = AVR32_GPIO.port[bport[i]].pvr & bpin[i];
				volatile avr32_gpio_port_t * led = &AVR32_GPIO.port[lport[i]];
				led->ovrs = lbit[i];
				vTaskResume(subTask[i]);
				pressed = FALSE;
			}
		}
	}
	

}

void tskSubTask(void* ptr){
	xTaskHandle task  = *(xTaskHandle *)ptr;
	while(TRUE){
		vTaskSuspend(NULL);
		vTaskSuspend(task);
		vWriteLine("Task Suspended!\n");
		vTaskDelay(5000);
		vTaskResume(task);
		vWriteLine("Task Resumed!\n");
	}

}

void vWriteLine(char* str){
	taskENTER_CRITICAL();
	usart_write_line(configDBG_USART , str);
	taskEXIT_CRITICAL();
}
