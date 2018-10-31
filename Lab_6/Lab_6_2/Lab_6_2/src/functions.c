/*
 * functions.c
 *
 * Created: 2018-10-30 16:03:59
 *  Author: sbn14007
 */ 

#include "define.h"

const int bport[3] = {BUTTON_PORT0,BUTTON_PORT1,BUTTON_PORT2};
const int bpin[3] = {BUTTON_PIN0,BUTTON_PIN1,BUTTON_PIN2};
xSemaphoreHandle fillCount;			// items produced
xSemaphoreHandle emptyCount;		// remaining space
xSemaphoreHandle WriteToDisplay;	// display
int total_chars = 0;
int status = FALSE;

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

void vWriteLine(char* str){
	taskENTER_CRITICAL();
	usart_write_line(configDBG_USART , str);
	taskEXIT_CRITICAL();
}

void initSemaphore(void){
	fillCount = xSemaphoreCreateCounting(BUFFER_SIZE,0);
	emptyCount = xSemaphoreCreateCounting(BUFFER_SIZE,BUFFER_SIZE);
	vSemaphoreCreateBinary(WriteToDisplay);
}

void tskReceiver(void* ptr)
{
	char c;
	if(xSemaphoreTake(WriteToDisplay,portMAX_DELAY));
	dip204_set_cursor_position(1,1);
	dip204_write_string("Received: ");
	xSemaphoreGive(WriteToDisplay);

	while(TRUE){
		c = usart_getchar(configDBG_USART);
		if(c!=NULL)total_chars += 1;
		if(xSemaphoreTake(WriteToDisplay,portMAX_DELAY));
		dip204_set_cursor_position(11,1);
		dip204_write_data(c);
		xSemaphoreGive(WriteToDisplay);
		vTaskDelay(10);
	}	
}

void tskStatus(void* ptr)
{
	if(xSemaphoreTake(WriteToDisplay,portMAX_DELAY));

	xSemaphoreGive(WriteToDisplay);
	
	while(TRUE){
		if(status == TRUE){
			if(xSemaphoreTake(WriteToDisplay,portMAX_DELAY));
			dip204_set_cursor_position(1,2);
			dip204_write_string("Total: ");
			dip204_set_cursor_position(8,2);
			dip204_write_data('0'+(total_chars/100));
			dip204_set_cursor_position(9,2);
			dip204_write_data('0'+(total_chars/10)%10);
			dip204_set_cursor_position(10,2);
			dip204_write_data('0'+(total_chars%10));
			xSemaphoreGive(WriteToDisplay);
		}
		vTaskDelay(10);
	}
}

void tskButton(void* ptr)
{
	while(TRUE){
		vTaskDelay(10);
		if(!(AVR32_GPIO.port[BUTTON_PORT0].pvr & bpin[BUTTON_PIN0])){
			status = TRUE;
			vTaskDelay(10000);
			status = FALSE;
			if(xSemaphoreTake(WriteToDisplay,portMAX_DELAY));
			dip204_set_cursor_position(1,2);
			dip204_write_string("                    ");
			xSemaphoreGive(WriteToDisplay);
		}
	}
}