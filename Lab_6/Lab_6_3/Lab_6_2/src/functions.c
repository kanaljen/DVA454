/*
 * functions.c
 *
 * Created: 2018-10-30 16:03:59
 *  Author: sbn14007
 */ 

#include "define.h"

const int bport[3] = {BUTTON_PORT0,BUTTON_PORT1,BUTTON_PORT2};
const int bpin[3] = {BUTTON_PIN0,BUTTON_PIN1,BUTTON_PIN2};
xSemaphoreHandle SemPot;
xSemaphoreHandle SemTemp;
xSemaphoreHandle SemLight;
xQueueHandle PotentQueue;
xQueueHandle TempQueue;
xQueueHandle LightQueue;

void USART_init(void)
{
	static const gpio_map_t USART_SERIAL_GPIO_MAP =
	{
		{ serialPORT_USART_RX_PIN , serialPORT_USART_RX_FUNCTION },
		{ serialPORT_USART_TX_PIN , serialPORT_USART_TX_FUNCTION }
	};
	static const gpio_map_t USART_DEBUG_GPIO_MAP =
	{
		{ configDBG_USART_RX_PIN , configDBG_USART_RX_FUNCTION } ,
		{ configDBG_USART_TX_PIN , configDBG_USART_TX_FUNCTION }
	};
	static const usart_options_t USART_OPTIONS =
	{
		. baudrate = 115200,
		. charlength = 8,
		. paritytype = USART_NO_PARITY ,
		. stopbits = USART_1_STOPBIT ,
		. channelmode = USART_NORMAL_CHMODE
	};
	pm_switch_to_osc0 (& AVR32_PM , FOSC0 , OSC0_STARTUP );
	gpio_enable_module ( USART_SERIAL_GPIO_MAP , 2);
	gpio_enable_module ( USART_DEBUG_GPIO_MAP , 2);
	usart_init_rs232 ( serialPORT_USART , & USART_OPTIONS , FOSC0 );
	usart_init_rs232 ( configDBG_USART , & USART_OPTIONS , FOSC0 );
	
	vWriteLine("USART Initialized\n");
}

void vWriteLine(char* str){
	taskENTER_CRITICAL();
	usart_write_line(configDBG_USART , str);
	taskEXIT_CRITICAL();
}

void initSemaphore(void){
	vSemaphoreCreateBinary(SemPot);
	vSemaphoreCreateBinary(SemTemp);
	vSemaphoreCreateBinary(SemLight);
}

void initQueues(void){
	PotentQueue = xQueueCreate(BUFFER_SIZE, sizeof(uint32_t));
	TempQueue = xQueueCreate(BUFFER_SIZE, sizeof(uint32_t));
	LightQueue = xQueueCreate(BUFFER_SIZE, sizeof(uint32_t));
}

void tskLight(void* ptr)
{
	volatile uint32_t light_value = 0;
	
	while(TRUE)
	{
		// Start a ADC sampling of all active channels
		adc_start(&AVR32_ADC);

		// Get value
		light_value = adc_get_value(&AVR32_ADC, ADC_LIGHT_CHANNEL);

		// Send to Queue
		if(xSemaphoreTake(SemLight,10)){
			xQueueSendToBack(LightQueue,&light_value,10);
			xSemaphoreGive(SemLight);
		}
		vTaskDelay(100);

	}
}

void tskPotent(void* ptr)
{	
	volatile uint32_t pot_value = 0;
	
	while(TRUE)
	{
		// Start a ADC sampling of all active channels
		adc_start(&AVR32_ADC);

		// Get value
		pot_value = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
		
		// Send to Queue
		if(xSemaphoreTake(SemPot,10)){
			xQueueSendToBack(PotentQueue,&pot_value,10);
			xSemaphoreGive(SemPot);
		}
		vTaskDelay(100);
	}

}

void tskTemp(void* ptr)
{
	volatile uint32_t temp_value = 0;
	
	while(TRUE)
	{
		// Start a ADC sampling of all active channels
		adc_start(&AVR32_ADC);

		// Value
		temp_value = adc_get_value(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
		
		// Send to Queue
		if(xSemaphoreTake(SemTemp,10)){
			xQueueSendToBack(TempQueue,&temp_value,10);
			xSemaphoreGive(SemTemp);
		}
		vTaskDelay(100);

	}

}

void tskDisplay(void* ptr){
	
	volatile uint32_t values[3] = {0,0,0}; // pot_value = 0, temp_value = 0, light_value = 0
	dip204_set_cursor_position(1,1);
	dip204_write_string("Potent:");
	dip204_set_cursor_position(1,2);
	dip204_write_string("Temp:");
	dip204_set_cursor_position(1,3);
	dip204_write_string("Light:");
	while(TRUE){
		if(xSemaphoreTake(SemPot,10)){
			xQueueReceive(PotentQueue, &values[0],10);
			xSemaphoreGive(SemPot);			
		}
		if(xSemaphoreTake(SemTemp,10)){
			xQueueReceive(TempQueue, &values[1],10);
			xSemaphoreGive(SemTemp);
		}
		if(xSemaphoreTake(SemLight,10)){
			xQueueReceive(LightQueue, &values[2],10);
			xSemaphoreGive(SemLight);
		}
		for(int i = 0; i < 3;i++){
			dip204_set_cursor_position(8,i+1);
			dip204_write_data('0'+(values[i]/1000));
			dip204_set_cursor_position(9,i+1);
			dip204_write_data('0'+(values[i]/100)%10);
			dip204_set_cursor_position(10,i+1);
			dip204_write_data('0'+(values[i]/10)%10);
			dip204_set_cursor_position(11,i+1);
			dip204_write_data('0'+(values[i]%10));
		}
		vTaskDelay(10);
	}
}