#include "functions.h"
int charCount = 0;
xSemaphoreHandle xSem = NULL;
char message[60];
int msglen;

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
	usart_init_rs232(configDBG_USART , &USART_OPTIONS , FOSC0);
	
	usart_write_line(configDBG_USART , "USART Initialized\n");
	vSemaphoreCreateBinary(xSem);
	usart_write_line(configDBG_USART, "Semaphore Created\n");
}
void vDisplayPrintMSG(void)
{
	/* This function is used in the get msg task to print the msg.
	   It first clears the lines so nothing old is left and prints
	   your msg to the serial monitor */
	
	usart_write_line(configDBG_USART , message);
	usart_write_line(configDBG_USART , "\n");
	
	/* Since more than once task manipulates the cursor position, 
	   this part is protected by a semaphore. */
	
	xSemaphoreTake(xSem, (portTickType) portMAX_DELAY);
		
	dip204_set_cursor_position(1,1);
	dip204_write_string("                    ");
	dip204_write_string("                    ");
	dip204_write_string("                    ");
	dip204_set_cursor_position(1,1);
	dip204_write_string(message);
	
	xSemaphoreGive(xSem);
}
void vGetMSGTASK(void* pvParameters)
{
	int i = 0;
	
	while(1)
	{
		/* Wait until there is something at the serial to read */
		while(!(USART->CSR.rxrdy));
		
		/* Since multiple task use the charCount, this part is proctected by a semaphore */
		
		xSemaphoreTake(xSem, (portTickType) portMAX_DELAY);
		
		/* Reads every char and puts it into a buffer, stops when it reaches '\n'
		   which means this must be added in the serial monitor
		   
		   For each character read, it will add 1 to the charCount to keep track of 
		   the amount of characters written */
		
		do{
			if(USART->CSR.rxrdy)
			{
				message[i] = USART->RHR.rxchr;
				i++;
				charCount++;
			}
		}
		while(message[i-1] != '\n');
				
		/* Subtract 2 to exclude \r\n to the charCount */		
				
		charCount = charCount - 2;
		
		xSemaphoreGive(xSem);
		
		/* We want each msg to end with \0 */
		
		message[i-2] = '\0';
		i = 0;
		
		vDisplayPrintMSG();
	}
}
void vButtonTASK(void* pvParameters)
{
	/* This task wait until a button is pressed, then prints the total amount 
	   of characters for 10 seconds and updates this continuously if more comes */
	
	volatile int button_state0; //Initialize button state 0
	xTaskHandle task  = *(xTaskHandle *)pvParameters;
	portTickType xLastWakeTime;
	const portTickType delay = 10000; //10 sec
	char str;
	portTickType startTick, stopTick;
	int currentCount = 9999;
	
	while(1){
		
		button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0; //Read button
		
		if(!button_state0){ //If button is pushed
			
			dip204_set_cursor_position(1, 4);
			sprintf(str, "#Char:");
			dip204_printf_string(str);
			startTick = xTaskGetTickCount();
			
			/* For 10000 ticks (10 s), prints charCount to LCD */
			do
			{   
				
				/* Only prints the charCount if it is updated or it is the first time
				   since a button event (currentCount = 9999 so they wont match first time) */
				if(currentCount != charCount)
				{
					/* Since we move the cursor and use the charCount, this part is protected 
					   by semaphore */
					
					xSemaphoreTake(xSem, (portTickType) portMAX_DELAY);			
					sprintf(str, "%d", charCount);
							
					dip204_set_cursor_position(8, 4);
					dip204_printf_string(str);
					
					currentCount = charCount;
					
					xSemaphoreGive(xSem);
				}
				
				stopTick = xTaskGetTickCount();
			} while ((stopTick - startTick) < 10000);
			
			/* Reset currentCount so it will not be equal to charCount the next button event */
			currentCount = 9999;
			
			dip204_set_cursor_position(1,4);
			dip204_write_string("                    ");
		}
	}
}


