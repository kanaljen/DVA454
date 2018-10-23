#include "functions.h"

int buffer[buffer_size];
int itemCount = 0;
xQueueHandle xQueueTemp, xQueuePot, xQueueLS;

/*struct msg
{
	int value;
	portTickType timestamp;	
};*/

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
	
	int PotMsg, TempMsg, LSMsg;
	char pot[8];
	char temp[8];
	char LS[8];
	char counter[8];
	int k = 0;
	
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
		xQueueReceive(xQueuePot, &PotMsg, (portTickType) 10);
		xQueueReceive(xQueueTemp, &TempMsg, (portTickType) 10);
		xQueueReceive(xQueueLS, &LSMsg, (portTickType) 10);
		
		sprintf(pot, "%d", PotMsg);
		sprintf(temp, "%d", TempMsg);
		sprintf(LS, "%d", LSMsg);
		sprintf(counter, "ctr: %d", k);
		
		dip204_set_cursor_position(6, 2);
		dip204_write_string(pot);
		dip204_set_cursor_position(6, 3);
		dip204_write_string(temp);
		dip204_set_cursor_position(6, 4);
		dip204_write_string(LS);
		dip204_set_cursor_position(12, 2);
		dip204_write_string(counter);
		
		k++;
	}
}
void vPotTask(void  *pvParameters)
{
	int msg; 
	
	while(1)
	{
		msg = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL); //Read potentiometer value
		//message.timestamp = xTaskGetTickCount();
		xQueueSend(xQueuePot, &msg, (portTickType) 10);
	}
}
void vTempTask(void *pvParameters)
{
	int msg;
	
	while(1)
	{
		msg = adc_get_value(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL); //Read potentiometer value
		//message.timestamp = xTaskGetTickCount();
		xQueueSend(xQueuePot, &msg, (portTickType) 10);
	}
}
void vLSTask(void *pvParameters)
{
	int msg;
	
	while(1)
	{
		msg = adc_get_value(&AVR32_ADC, ADC_LIGHT_CHANNEL); //Read potentiometer value
		//message.timestamp = xTaskGetTickCount();
		xQueueSend(xQueuePot, &msg, (portTickType) 10);
	}
}