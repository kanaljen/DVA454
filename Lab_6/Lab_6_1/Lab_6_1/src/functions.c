/*
 * functions.c
 *
 * Created: 2018-10-30 16:03:59
 *  Author: sbn14007
 */ 

#include "define.h"

const int bport[3] = {BUTTON_PORT0,BUTTON_PORT1,BUTTON_PORT2};
const int bpin[3] = {BUTTON_PIN0,BUTTON_PIN1,BUTTON_PIN2};
const int lport[3] = {LED0_PORT,LED1_PORT,LED2_PORT};
const int lbit[3] = {LED0_BIT_VALUE,LED1_BIT_VALUE,LED2_BIT_VALUE};
int buffer = 0; 
xSemaphoreHandle fillCount; // items produced
xSemaphoreHandle emptyCount; // remaining space

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
	vWriteLine("LED:s Initialized\n");
}

void vWriteLine(char* str){
	taskENTER_CRITICAL();
	usart_write_line(configDBG_USART , str);
	taskEXIT_CRITICAL();
}

void initSemaphore(void){
	fillCount = xSemaphoreCreateCounting(BUFFER_SIZE,0);
	emptyCount = xSemaphoreCreateCounting(BUFFER_SIZE,BUFFER_SIZE);
}

void tskProducer(void* ptr)
{	
	int byte = 0;
	xTaskHandle* ConsTaskHandle = *(xTaskHandle*)ptr;
	
	while(TRUE){
		/* Solution */
		
		byte = 1; // Produce byte
		vWriteLine("P: +1\n");
		//xSemaphoreTake(emptyCount,portMAX_DELAY);
		if(buffer>BUFFER_SIZE-1){
			vWriteLine("P: Buffer full, going to sleep\n");
			vTaskSuspend(NULL);
			vWriteLine("P: Awake\n");
		}
		buffer += byte; // Store byte in buffer
		//xSemaphoreGive(fillCount);
		byte = 0;
		if(buffer == 1)vTaskResume(ConsTaskHandle);
		
		/* Solution */

	}
}

void tskConsumer(void* ptr)
{
	int byte = 0;
	xTaskHandle *ProdTaskHandle = *(xTaskHandle*)ptr;
	while(TRUE){
		if(buffer == 0){
			vWriteLine("C: Buffer empty, going to sleep\n");
			vTaskSuspend(NULL);
			vWriteLine("C: Awake\n");
		}
		byte = 1; // Remove byte from buffer
		//xSemaphoreTake(fillCount,portMAX_DELAY);
		buffer -= byte;
		vWriteLine("C: -1\n");
		if(buffer == BUFFER_SIZE-1)vTaskResume(ProdTaskHandle);
		//xSemaphoreGive(emptyCount);
		byte = 0; // Consume byte
		

	}
}