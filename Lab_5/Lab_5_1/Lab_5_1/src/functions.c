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
	
	vWriteLine("USART Initialized\n");
}

void LED_init(void)
{
	volatile avr32_gpio_port_t * led0_port = &AVR32_GPIO.port[LED0_PORT];
	led0_port->gpers = LED0_BIT_VALUE; //Set Enable register
	led0_port->ovrs = LED0_BIT_VALUE; //Set output value register
	led0_port->oders = LED0_BIT_VALUE; //Set output drive register
	vWriteLine("LED 0 Initialized\n");
	
	volatile avr32_gpio_port_t * led1_port = &AVR32_GPIO.port[LED1_PORT];
	led1_port->gpers = LED1_BIT_VALUE; //Set Enable register
	led1_port->ovrs = LED1_BIT_VALUE; //Set output value register
	led1_port->oders = LED1_BIT_VALUE; //Set output drive register
	vWriteLine("LED 1 Initialized\n");
	
	volatile avr32_gpio_port_t * led2_port = &AVR32_GPIO.port[LED2_PORT];
	led2_port->gpers = LED2_BIT_VALUE; //Set Enable register
	led2_port->ovrs = LED2_BIT_VALUE; //Set output value register
	led2_port->oders = LED2_BIT_VALUE; //Set output drive register
	vWriteLine("LED 2 Initialized\n");
	
	vWriteLine("All LED:s Initialized\n");
	
}

void vBlinkLED0( void * pvParameters )
{
	volatile avr32_gpio_port_t * led0_port = &AVR32_GPIO.port[LED0_PORT];
    while(TRUE)
    {
		vWriteLine("LED 0 Blinking\n");
		vTaskDelay(1000);
        led0_port->ovrt = LED0_BIT_VALUE;
    }
}

void vBlinkLED1( void * pvParameters )
{
	volatile avr32_gpio_port_t * led1_port = &AVR32_GPIO.port[LED1_PORT];
    while(TRUE)
    {
		vWriteLine("LED 2 Blinking\n");
	    vTaskDelay(2000);
	    led1_port->ovrt = LED1_BIT_VALUE;
    }
}

void vBlinkLED2( void * pvParameters )
{
	volatile avr32_gpio_port_t * led2_port = &AVR32_GPIO.port[LED2_PORT];
    while(TRUE)
    {
		vWriteLine("LED 3 Blinking\n");
	    vTaskDelay(3000);
	    led2_port->ovrt = LED2_BIT_VALUE;
    }
}

void vButtons( void * pvParameters ){
	
	int state[3] = {1,1,1};
	const int bport[3] = {BUTTON_PORT0,BUTTON_PORT1,BUTTON_PORT2};
	const int bpin[3] = {BUTTON_PIN0,BUTTON_PIN1,BUTTON_PIN2};
	const int lport[3] = {LED0_PORT,LED1_PORT,LED2_PORT};
	const int lbit[3] = {LED0_BIT_VALUE,LED1_BIT_VALUE,LED2_BIT_VALUE};
	int pressed = FALSE;
	xTaskHandle* task = pvParameters;
	xTaskHandle* subTask = malloc(sizeof(xTaskHandle*)*3);
	xTaskCreate(vSubTask,"subTask0",STACK_SIZE,&(task[0]),1,&(subTask[0]));
	xTaskCreate(vSubTask,"subTask1",STACK_SIZE,&(task[1]),1,&(subTask[1]));
	xTaskCreate(vSubTask,"subTask2",STACK_SIZE,&(task[2]),1,&(subTask[2]));
	

	while (TRUE)
	{
		for(int i=0;i<3;i++){
			state[i] = AVR32_GPIO.port[bport[i]].pvr & bpin[i]; 
			if(!state[i] && pressed == FALSE){
				pressed = TRUE;
				vWriteLine("Button pushed!\n");
				while(!state[i])state[i] = AVR32_GPIO.port[bport[i]].pvr & bpin[i];
				volatile avr32_gpio_port_t * led = &AVR32_GPIO.port[lport[i]];
				led->ovrc = lbit[i];
				vTaskResume(subTask[i]);
				pressed = FALSE;
			}
		}
	}
	

}

void vSubTask( void * pvParameters){
	xTaskHandle task  = *(xTaskHandle *)pvParameters;
	while(TRUE){
		vTaskSuspend(NULL);
		vTaskSuspend(task);
		vWriteLine("Task Suspended!\n");
		vTaskDelay(5000);
		vTaskResume(task);
		vWriteLine("Task Resumed!\n");
	}

}

void vWriteLine(char* line){
	taskENTER_CRITICAL();
	usart_write_line(configDBG_USART , line);
	taskEXIT_CRITICAL();
}
