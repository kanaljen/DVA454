#include "functions.h"

int buffer[buffer_size];
int itemCount = 0;
xSemaphoreHandle xSem = NULL;

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

void vUSARTTask(void* pvParameters) {
	
		xTaskHandle task  = *(xTaskHandle *)pvParameters; //Blink LED handle
	
	while(1) {
		usart_read_char(configDBG_USART,)
		
	}
}

void vLCDTask(void* pvParameters) {
	while(1) { 
			vTaskSuspend(NULL); 
			dip204_set_cursor_position(1, 1);
			dip204_printf_string("HELLO WORLD!");
			
	}
}
void vButtonTASK(void* pvParameters)
{
	volatile int button_state0; //Initialize button state 0
	xTaskHandle task  = *(xTaskHandle *)pvParameters; //Blink LED handle
	
	while(1){
		
		while(button_state0) //While button isn't pushed
		button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0; //Read button
		
		if(!button_state0){ //If button is pushed
			taskENTER_CRITICAL(); //Makes sure the message isn't being over written
			usart_write_line(configDBG_USART , "Button 0 pressed\n");
			taskEXIT_CRITICAL();
			
			vTaskResume(task); //Suspend the task with the same LED with its handle
			
			while(!button_state0) //If the button is still pressed, it will not keep running the task
			button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0; //Read Button 0
		}
	}
}