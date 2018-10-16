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
	xTaskHandle ConsumerTaskHandle = *(xTaskHandle *)pvParameters;
	int item;
	char char_buffer[16];
	
	while(1)
	{	
		if(itemCount = buffer_size)
		{
			vTaskSuspend(NULL);
			usart_write_line(configDBG_USART, "Producer Sleeping\n");
		}  
		
		buffer[itemCount] = itemCount + 1;
		item = buffer[itemCount];
		itemCount++;
		
		if(itemCount > 0)
		{
			vTaskResume(ConsumerTaskHandle);
			usart_write_line(configDBG_USART, "Producer Awaken\n");
		}
		
		sprintf(char_buffer, "Item produced: %d\n", item);
		usart_write_line(configDBG_USART , char_buffer);
		
	}
}
void vConsumerTask(void* pvParameters)
{
	
	xTaskHandle ProducerTaskHandle = *(xTaskHandle *)pvParameters;
	int item;
	char char_buffer[16];
	
	while(1)
	{
		if(itemCount == 0)
		{
			vTaskSuspend(NULL);
			usart_write_line(configDBG_USART, "Consumer Sleeping\n");
		}
		
		item = buffer[itemCount];
		buffer[itemCount] = NULL;
		itemCount--;
		
		if(itemCount < buffer_size)
		{
			vTaskResume(ProducerTaskHandle);
			usart_write_line(configDBG_USART, "Consumer Awaken\n");
		}
		
		sprintf(char_buffer, "Item consumed: %d\n", item);
		usart_write_line(configDBG_USART , char_buffer);
		
	}
}
