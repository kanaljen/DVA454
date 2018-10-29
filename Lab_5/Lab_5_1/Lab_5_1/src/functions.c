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
	
	usart_write_line(configDBG_USART , "USART Initialized\n");
}

void LED_init(void)
{
	volatile avr32_gpio_port_t * led0_port = &AVR32_GPIO.port[LED0_PORT];
	led0_port->gpers = LED0_BIT_VALUE; //Set Enable register
	led0_port->ovrs = LED0_BIT_VALUE; //Set output value register
	led0_port->oders = LED0_BIT_VALUE; //Set output drive register
	usart_write_line(configDBG_USART , "LED 0 Initialized\n");
	
	volatile avr32_gpio_port_t * led1_port = &AVR32_GPIO.port[LED1_PORT];
	led1_port->gpers = LED1_BIT_VALUE; //Set Enable register
	led1_port->ovrs = LED1_BIT_VALUE; //Set output value register
	led1_port->oders = LED1_BIT_VALUE; //Set output drive register
	usart_write_line(configDBG_USART , "LED 1 Initialized\n");
	
	volatile avr32_gpio_port_t * led2_port = &AVR32_GPIO.port[LED2_PORT];
	led2_port->gpers = LED2_BIT_VALUE; //Set Enable register
	led2_port->ovrs = LED2_BIT_VALUE; //Set output value register
	led2_port->oders = LED2_BIT_VALUE; //Set output drive register
	usart_write_line(configDBG_USART , "LED 2 Initialized\n");
	
	usart_write_line(configDBG_USART , "All LED:s Initialized\n");
	
}

void vBlinkLED0( void * pvParameters )
{
	volatile avr32_gpio_port_t * led0_port = &AVR32_GPIO.port[LED0_PORT];
    for( ;; )
    {
		vTaskDelay(1000);
        led0_port->ovrt = LED0_BIT_VALUE;
    }
}

void vBlinkLED1( void * pvParameters )
{
	portTickType xLastWakeTime;
	const portTickType xFrequency = 2000;
	xLastWakeTime = xTaskGetTickCount();
	volatile avr32_gpio_port_t * led1_port = &AVR32_GPIO.port[LED1_PORT];
	for( ;; )
	{

		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		led1_port->ovrt = LED1_BIT_VALUE;
	}
}

void vBlinkLED2( void * pvParameters )
{
	portTickType xLastWakeTime;
	const portTickType xFrequency = 3000;
	xLastWakeTime = xTaskGetTickCount();
	volatile avr32_gpio_port_t * led2_port = &AVR32_GPIO.port[LED2_PORT];
	for( ;; )
	{
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		led2_port->ovrt = LED2_BIT_VALUE;
	}
}

void vButtons( void * pvParameters ){
	
	int state[3] = {1,1,1};
	int port[3] = {BUTTON_PORT0,BUTTON_PORT1,BUTTON_PORT2};
	int pin[3] = {BUTTON_PIN0,BUTTON_PIN1,BUTTON_PIN2};
	xTaskHandle* task = pvParameters;
	

	while (TRUE)
	{
		for(int i=0;i < 3;i++){
			state[i] = AVR32_GPIO.port[port[i]].pvr & pin[i];
			if(!state[i]){
				vTaskSuspend(task[i]); //Suspend the task with the same LED with its handle
			}
		}
	}
	

}
