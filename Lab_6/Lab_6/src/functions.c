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
void vSemaphoreTask( void * pvParameters )
{
	vSemaphoreCreateBinary(xSem);
	vTaskSuspend(NULL);
}

void vProducerTask(void* pvParameters)
{
	xTaskHandle ConsumerTaskHandle = *(xTaskHandle *)pvParameters;
	int item;
	char char_buffer[16];
		
	while(1)
	{	
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
		
		if(xSem != NULL)
		{
			while(xSemaphoreTake(xSem, (portTickType) 0) != pdPASS);
			taskENTER_CRITICAL();
			usart_write_line(configDBG_USART, "Producer took Semaphore\n");
			taskEXIT_CRITICAL();
		}
		
		buffer[itemCount] = itemCount + 1;
		item = buffer[itemCount];
		itemCount++;
		
		taskENTER_CRITICAL();
		usart_write_line(configDBG_USART, "Item added to buffer\n");
		usart_write_line(configDBG_USART, "Producer gave Semaphore\n");
		taskEXIT_CRITICAL();
		xSemaphoreGive(xSem);
		
		if(itemCount == 1)
		{
			vTaskResume(ConsumerTaskHandle);
		}
		
		sprintf(char_buffer, "Item %d produced\n", item);
		taskENTER_CRITICAL();
		usart_write_line(configDBG_USART , char_buffer);
		taskEXIT_CRITICAL();
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
			for(int i = 0; i < 100; i++);
			taskENTER_CRITICAL();
			usart_write_line(configDBG_USART, "Consumer Sleeping\n");
			taskEXIT_CRITICAL();
			vTaskSuspend(NULL);
			taskENTER_CRITICAL();
			usart_write_line(configDBG_USART, "Consumer Awakened\n");
			taskEXIT_CRITICAL();
		}
		
		if(xSem != NULL)
		{
			while(xSemaphoreTake(xSem, (portTickType) 0) != pdPASS);
			taskENTER_CRITICAL();
			usart_write_line(configDBG_USART, "Consumer took Semaphore\n");
			taskEXIT_CRITICAL();
		}
		
		item = buffer[itemCount-1];
		itemCount--;
		
		taskENTER_CRITICAL();
		usart_write_line(configDBG_USART, "Item removed from buffer\n");
		usart_write_line(configDBG_USART, "Consumer gave Semaphore\n");
		taskEXIT_CRITICAL();
		
		xSemaphoreGive(xSem);
		
		if(itemCount == buffer_size - 1)
		{
			vTaskResume(ProducerTaskHandle);
		}
		
		sprintf(char_buffer, "Item %d consumed\n", item);
		taskENTER_CRITICAL();
		usart_write_line(configDBG_USART , char_buffer);
		taskEXIT_CRITICAL();	
	}
}
