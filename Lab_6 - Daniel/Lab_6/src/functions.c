#include "functions.h"
xSemaphoreHandle xSem = NULL;
xQueueHandle xQueue;

void sensor_init(void)
{
	adc_configure(&AVR32_ADC);
	adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
	adc_enable(&AVR32_ADC, ADC_LIGHT_CHANNEL);
	
	usart_write_line(configDBG_USART , "Sensors Initialized\n");
}
void queue_init(void){
	xQueue = xQueueCreate(30, sizeof( struct Message));
	usart_write_line(configDBG_USART , "Queue Initialized\n");
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
void vLCDTASK(void* pvParameters)
{
	int pot_value;
	int tmp_value;
	int ldr_value;
	struct Message *msg;
	char pot[10];
	
	
	signed portBASE_TYPE ret;
	
	while(1)
	{
		
		// 		pot_value = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
		// 			usart_write_line(configDBG_USART , ("%d\n", pot_value));
		//xQueueReceive( xQueue, &(msg), (portTickType) 10 );
		//pot_value = msg->pot_value;
		
		
		// 			dip204_set_cursor_position(1,1);
		// 			dip204_write_string(msg->pot_value);
		
	}
}
void vPotTASK(void* pvParameters)
{
	
	int potvalue;
	struct Message *msg;
	const char buffer[16];
	
	while(1)
	{
		adc_start(&AVR32_ADC);
		potvalue = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL); //Read potentiometer value
		usart_write_line(configDBG_USART , "gay\n");
		
		sprintf(buffer, "POT: %d", potvalue);
		dip204_set_cursor_position(1, 2);
		dip204_printf_string(buffer);
		
		// 		msg->pot_value = pot_value;
		// 		sprintf(buffer, "%d", pot_value);
		//		usart_write_line(configDBG_USART , "då\n");
		// 		if( xQueue != NULL )
		// 						xQueueSend( xQueue, &msg, (portTickType) 10);
	}
}
void vTmpTASK(void* pvParameters){ 

	int tmp_value;
	char buffer[20];

	while(1)
	{
		//	   tmp_value = adc_get_value(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL); //Read temperature value
		
		
		// 		sprintf(buffer, "TMP: %d", tmp_value);
		//
		// 		dip204_set_cursor_position(1, 1);
		// 		dip204_printf_string(buffer);
		
		
		
		
	}
}
void vLdrTASK(void* pvParameters) {
	
	int ldr_value;
	char buffer[20];
	
	while(1)
	{
		//	   usart_write_line(configDBG_USART , "n\n");
		
		//	   ldr_value = adc_get_value(&AVR32_ADC, ADC_LIGHT_CHANNEL); //Read light sensor value
		
		// 	   sprintf(buffer, "LDR: %d", ldr_value);
		//
		// 	   dip204_set_cursor_position(1, 3);
		// 	   dip204_printf_string(buffer);
	}
}