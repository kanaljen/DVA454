#include "functions.h"
int count = 0;

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

void vLCDTask(void* pvParameters) {
	//char buffer[buffer_size];
	char single_char;
	int c, k, ret; 
	const portTickType delay = 1000; //10 sec
	portTickType xLastWakeTime;
	
	while(1) {
		
		
			k=1;		
	
		
		while (usart_read_char(configDBG_USART, &c) == USART_SUCCESS) {
			c=usart_getchar(configDBG_USART);
		
			sprintf(single_char, "%c", c);
			dip204_set_cursor_position(k, 1);
			dip204_printf_string(single_char);
			k++;
			count++;
		}
	
		
	}
}

void vButtonTASK(void* pvParameters)
{
	volatile int button_state0; //Initialize button state 0
	xTaskHandle task  = *(xTaskHandle *)pvParameters; //Blink LED handle
	portTickType xLastWakeTime;
	const portTickType delay = 10000; //10 sec
	char str;
	
	
	while(1){
		
		button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0; //Read button
		
		if(!button_state0){ //If button is pushed
			usart_write_line(configDBG_USART , "Button 0 pressed\n");
			

			dip204_set_cursor_position(1, 4);
			sprintf(str, "#Char:");
			dip204_printf_string(str);
			
			vTaskResume(task);
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, delay); //Delays 10 sec
			vTaskSuspend(task);
			
			dip204_clear_display();
		}
	}
}

void vCountTASK(void* pvParameters) {


	char str;
	vTaskSuspend(NULL);
	while(1){
		sprintf(str, "%d", count);
		dip204_set_cursor_position(8, 4);
		dip204_printf_string(str);
	}
}
		