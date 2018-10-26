#include "functions.h"

#define sample_size 10
xQueueHandle xQueueTemp, xQueuePot, xQueueLS;

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
		.baudrate = 921600,
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
void sensor_init(void)
{
	adc_configure(&AVR32_ADC);
	adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_LIGHT_CHANNEL);
	
}
void queue_init(void)
{
	xQueuePot = xQueueCreate(10, sizeof(int));
	xQueueTemp = xQueueCreate(10, sizeof(int));
	xQueueLS = xQueueCreate(10, sizeof(int));
}
void vDisplayTask(void)
{
	/* This task communicates with sensor input task and display
	the values it gets from those with the help of msg queues */
	
	int PotValue = 0, TempValue = 0, LSValue = 0;
	char pot[8];
	char temp[8];
	char LS[8];
	char USART_buffer[32];
	int k = 0;
	
	/* Initial print to LCD */
	
	dip204_set_cursor_position(1,1);
	dip204_write_string("Sensors Monitor 2000");
	dip204_set_cursor_position(1,2);
	dip204_write_string("Pot: ");
	dip204_set_cursor_position(1,3);
	dip204_write_string("Tmp: ");
	dip204_set_cursor_position(1,4);
	dip204_write_string(" LS: ");
	
	while(1)
	{
		/* For each queue, check if there is a message waiting
		   if so, wait to receive the message then print it. 
		   Each 10 times it will clear the space so nothing old is left */
		
		if (uxQueueMessagesWaiting( xQueuePot ) > 0) //Check if there is a message
		{
			xQueueReceive(xQueuePot, &PotValue, (portTickType) 5); //Receive the message
			
			/* Print the message and reset screen each 10 time */
			
			sprintf(pot, "%d", PotValue);
			if(k % 10)
			{
				dip204_set_cursor_position(6, 2);
				dip204_write_string("    ");
			}
			dip204_set_cursor_position(6, 2);
			dip204_write_string(pot);
		}
		
		/* Repeat above for rest of the sensors and queues */
		
		if (uxQueueMessagesWaiting( xQueueTemp ) > 0)
		{
			xQueueReceive(xQueueTemp, &TempValue, (portTickType) 20);
			
			sprintf(temp, "%d", TempValue);
			if(k % 10)
			{
				dip204_set_cursor_position(6, 3);
				dip204_write_string("    ");
			}
			dip204_set_cursor_position(6, 3);
			dip204_write_string(temp);
		}
		
		if (uxQueueMessagesWaiting( xQueueLS ) > 0)
		{
			xQueueReceive(xQueueLS, &LSValue, (portTickType) 10);
			
			sprintf(LS, "%d", LSValue);
			if(k % 10)
			{
				dip204_set_cursor_position(6, 4);
				dip204_write_string("    ");
			}
			dip204_set_cursor_position(6, 4);
			dip204_write_string(LS);
		}
		
		/* Prints the message to the serial monitor, must use external that can handle ANSII */
		
		sprintf(USART_buffer, "\033[2J\033cPot: %d Tmp: %d LS: %d", PotValue, TempValue, LSValue);
		usart_write_line(configDBG_USART, USART_buffer);
		
		k++;
	}
}
void vPotTask(void  *pvParameters)
{
	/* PotTask, TempTask and LSTask work similarly, only different ADC channels.
	
	   The task takes X samples from the sensor and averages this, the average value is sent 
	   to the corresponding queue	*/
	
	int value, i;
	int total_value = 0;
	int samples = sample_size;
	
	while(1)
	{
		for(i = 0; i < samples; i++)
		{
			adc_start(&AVR32_ADC);
			value = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL); //Read potentiometer value
			total_value = total_value + value;                            //Add to total for X samples
		}
		
		/* Compute the average and put it in corresponding queue */
		
		value = total_value/samples;
		total_value = 0;
		xQueueSend(xQueuePot, &value, (portTickType) 5);
		
	}
}
void vTempTask(void *pvParameters)
{
	int value, i;
	int total_value = 0;
	int samples = sample_size;
	
	while(1)
	{
		for(i = 0; i < samples; i++)
		{
			adc_start(&AVR32_ADC);
			value = adc_get_value(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL); //Read temperature value
			total_value = total_value + value;
		}
		value = total_value/samples;
		total_value = 0;
		xQueueSend(xQueueTemp, &value, (portTickType) 20);
	}
}
void vLSTask(void *pvParameters)
{
	int value, i;
	int total_value = 0;
	int samples = sample_size;
	
	while(1)
	{
		for(i = 0; i < samples; i++)
		{
			adc_start(&AVR32_ADC);
			value = adc_get_value(&AVR32_ADC, ADC_LIGHT_CHANNEL); //Read light sensor value
			total_value = total_value + value;
		}
		value = total_value/samples;
		total_value = 0;
		xQueueSend(xQueueLS, &value, (portTickType) 10);
		
	}
}