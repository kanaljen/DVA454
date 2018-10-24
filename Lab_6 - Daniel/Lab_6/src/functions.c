#include "functions.h"
xQueueHandle xQueue = NULL;

struct Message
{
	int tmp;
	int pot;
	int ldr;	
};

void SENSOR_init(void)
{
	adc_configure(&AVR32_ADC);
	adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_LIGHT_CHANNEL);
	
	usart_write_line(configDBG_USART , "Sensors Initialized\n");
}
void QUEUE_init(void)
{
	
	xQueue = xQueueCreate(10, sizeof(struct Message));
	usart_write_line(configDBG_USART, "Queue Initialized\n");
}
void usart_init(void)
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
void vQueueTASK(void)
{
	struct Message *msg;
	char pot[16];
	char tmp[16];
	char ldr[16];
	
	
	signed portBASE_TYPE ret;
	
	while(1)
	{
		taskENTER_CRITICAL();
		usart_write_line(configDBG_USART , "TEST1\n");
		taskEXIT_CRITICAL();
		
		if(xQueue != NULL) usart_write_line(configDBG_USART, "NULL \n");
		else usart_write_line(configDBG_USART, "NOT NULL!! \n");
		
		if (uxQueueMessagesWaiting( xQueue ) > 0)
		{
			usart_write_line(configDBG_USART, "IM HERE!! \n");
			xQueueReceive(xQueue, &msg, (portTickType) 10);
			
			if(msg->pot != 0)
			{
				sprintf(pot, "%d", msg->pot);
				dip204_set_cursor_position(6, 1);
				dip204_write_string("    ");
				dip204_set_cursor_position(6, 1);
				dip204_write_string(pot);
			}
			
			xQueueReceive(xQueue, &msg, (portTickType) 10);
			
			if(msg->pot != 0)
			{
				sprintf(tmp, "%d", msg->tmp);
				dip204_set_cursor_position(6, 2);
				dip204_write_string("    ");
				dip204_set_cursor_position(6, 2);
				dip204_write_string(tmp);
			}
		
			xQueueReceive(xQueue, &msg, (portTickType) 10);
			
			if(msg->pot != 0)
			{
				sprintf(ldr, "%d", msg->ldr);
				dip204_set_cursor_position(6, 3);
				dip204_write_string("    ");
				dip204_set_cursor_position(6, 3);
				dip204_write_string(ldr);
			}
		}
		else usart_write_line(configDBG_USART, "Queue EMTPTY\n");
		
	}
}
void vPotTASK(void)
{
	int i, pot_average;
	struct Message *pot_value;
	pot_value->ldr = 0;
	pot_value->tmp = 0;
	
	while(1)
	{
		taskENTER_CRITICAL();
		usart_write_line(configDBG_USART , "TEST2\n");
		taskEXIT_CRITICAL();
		for(i = 1; i <= 50; i++) // Collecting 50 samples
		{
			adc_start(&AVR32_ADC);
			pot_value->pot = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL); //Read potentiometer value
			pot_average = pot_average + pot_value->pot;
			usart_write_line(configDBG_USART , "TEST\n");
		}
		
		pot_average = pot_average / i;
		pot_value->pot = pot_average;
		pot_average=0;
		
		xQueueSend(xQueue, &pot_value, (portTickType) 10);
	}
}
void vTmpTASK(void)
{
	int i, tmp_average;
	struct Message *tmp_value;
	tmp_value->ldr = 0;
	tmp_value->pot = 0;
	
	while(1)
	{
		taskENTER_CRITICAL();
		usart_write_line(configDBG_USART , "TEST3\n");
		taskEXIT_CRITICAL();
		for(i = 1; i <= 50; i++) // Collecting 50 samples
		{
			adc_start(&AVR32_ADC);
			tmp_value->tmp = adc_get_value(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL); //Read potentiometer value
			tmp_average = tmp_average + tmp_value->tmp;
			usart_write_line(configDBG_USART , "TEST\n");
		}
		
		tmp_average = tmp_average / i;
		tmp_value->tmp = tmp_average;
		tmp_average=0;
		
		xQueueSend(xQueue, &tmp_value, (portTickType) 10);
	}
}
void vLdrTASK(void) 
{
	int i, ldr_average;
	struct Message *ldr_value;
	ldr_value->pot = 0;
	ldr_value->tmp = 0;
	
	while(1)
	{
		taskENTER_CRITICAL();
		usart_write_line(configDBG_USART , "TEST3\n");
		taskEXIT_CRITICAL();
		
		for(i = 1; i <= 50; i++) // Collecting 50 samples
		{
			adc_start(&AVR32_ADC);
			ldr_value->ldr = adc_get_value(&AVR32_ADC, ADC_LIGHT_CHANNEL); //Read potentiometer value
			ldr_average = ldr_average + ldr_value->ldr;
			usart_write_line(configDBG_USART , "TEST\n");
		}
		
		ldr_average = ldr_average / i;
		ldr_value->ldr = ldr_average;
		ldr_average=0;
		
		xQueueSend(xQueue, &ldr_value, (portTickType) 10);
	}
	
}