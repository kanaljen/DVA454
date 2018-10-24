#include "functions.h"

int buffer[buffer_size];
int itemCount = 0;

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

void vProducerTask(void* pvParameters)
{
	
	/* This function will produce "an item" and add it to a buffer
	   and when the buffer is full it will go to sleep, if the buffer 
	   has 1 item, it will wake the consumer */
	
	xTaskHandle ConsumerTaskHandle = *(xTaskHandle *)pvParameters;
	int item;
	char char_buffer[16];
	
	while(1)
	{
		/* The producer first checks if the buffer is full, if so it goes to sleep 
		   and gives an alert, same when it awakens */
		if(itemCount == buffer_size)
		{
			taskENTER_CRITICAL();
			usart_write_line(configDBG_USART, "Producer Sleeping\n");
			taskEXIT_CRITICAL();
			
			vTaskSuspend(NULL);
			
			taskENTER_CRITICAL();
			usart_write_line(configDBG_USART, "Producer Awakened\n");
			taskEXIT_CRITICAL();
		}
			
		/* The producer and consumer keeps track of the items they add 
		   or subtract with the "itemCount" variable */
		
		buffer[itemCount] = itemCount + 1;
		item = buffer[itemCount];
		itemCount++;
		
		/* If the buffer has 1 item, it is not empty and it will try to wake
		   the consumer with no regards to if it is sleeping or not */
		
		if(itemCount == 1)
		{
			vTaskResume(ConsumerTaskHandle);
		}
		
		/* CAUSION!! DO NOT ADD A " . " IN THE STRING BELOW */
		sprintf(char_buffer, "Item %d produced\n", item);
		taskENTER_CRITICAL();
		usart_write_line(configDBG_USART , char_buffer);
		taskEXIT_CRITICAL();
	}
}
void vConsumerTask(void* pvParameters)
{
	/* This function will consume "an item" from a buffer, if the buffer
	   is empty, it will go to sleep and if the buffer is not full it
	   will try to wake the producer */
	
	xTaskHandle ProducerTaskHandle = *(xTaskHandle *)pvParameters;
	int item;
	char char_buffer[16];
	
	while(1)
	{
		/* The consumer checks if there are any items to consume, otherwise
		   the consumer will go to sleep */
		
		if(itemCount == 0)
		{
			taskENTER_CRITICAL();
			usart_write_line(configDBG_USART, "Consumer Sleeping\n");
			taskEXIT_CRITICAL();
			
			vTaskSuspend(NULL);
			
			taskENTER_CRITICAL();
			usart_write_line(configDBG_USART, "Consumer Awakened\n");
			taskEXIT_CRITICAL();
		}
		
		/* Whenever the consumer consumes an item, it subtracts 1 from the
		   itemCount variable, when an item is removed it is replaced by 0 */
		
		item = buffer[itemCount-1];
		buffer[itemCount-1] = 0;
		itemCount--;
		
		/* If the buffer is not full, the consumer will try to wake the producer 
		   regardless of if it is sleeping or not */
		
		if(itemCount == buffer_size - 1)
		{
			vTaskResume(ProducerTaskHandle);
		}
		
		/* CAUSION!! DO NOT REMOVE THE " . " IN THE STRING BELOW */
		sprintf(char_buffer, "Item %d consumed.\n", item);
		taskENTER_CRITICAL();
		usart_write_line(configDBG_USART , char_buffer);
		taskEXIT_CRITICAL();
	}
}
