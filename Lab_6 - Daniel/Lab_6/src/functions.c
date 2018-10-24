#include "functions.h"
#define delay 10;

xQueueHandle xQueue = NULL;

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
	
	xQueue = xQueueCreate(30, sizeof(struct Message*));
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
		.baudrate = 921600, //High baudrate for the sensors
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
	char potv[8];
	int p, t, l;
	char tmpv[8];
	char ldrv[8];
	char buffer[32];
	
	//Initial LCD prints
	dip204_set_cursor_position(1,1);
	dip204_write_string("POT: ");
	dip204_set_cursor_position(1,2);
	dip204_write_string("TMP: ");
	dip204_set_cursor_position(1,3);
	dip204_write_string("LDR: ");
	
	while(1)
	{
		if (uxQueueMessagesWaiting( xQueue ) > 0) //If the queue is not empty
		{
			
			xQueueReceive(xQueue, &(msg), (portTickType) 20); //Get message from queue
			
			//Print POT values
			if(msg->id == 0)
			{
				//Transfer to string
				sprintf(potv, "%d", msg->pot); 
				
				//Clear line
				dip204_set_cursor_position(6, 1);
				dip204_write_string("    ");
				
				//Print new value
 				dip204_set_cursor_position(6, 1);
				dip204_write_string(potv);
				
				//Save the last value for serial print
				p=msg->pot;
			}
			
			//Print TMP values
			if(msg->id == 1)
			{
				//Transfer to string
 				sprintf(tmpv, "%d", msg->tmp);
				 
				 //Clear line
 				dip204_set_cursor_position(6, 2);
 				dip204_write_string("    ");
				 
				 //Print new value
 				dip204_set_cursor_position(6, 2);
 				dip204_write_string(tmpv);
				 
				 //Save the last value for serial print
				t=msg->tmp;
			}
			
			//Print LDR values
			if(msg->id == 2)
			{
				//Transfer to string
 				sprintf(ldrv, "%d", msg->ldr);
				 
				 //Clear line
 				dip204_set_cursor_position(6, 3);
 				dip204_write_string("    ");
				 
				 //Print new value
 				dip204_set_cursor_position(6, 3);
				dip204_write_string(ldrv);
				
				//Save the last value for serial print
				l=msg->tmp;
			}
			
			//Print the sensor values to serial with ANSII slear codes
			sprintf(buffer, "\033[2J\033cPot: %d Tmp: %d Ldr: %d", p, t, l);
 			usart_write_line(configDBG_USART ,buffer);
		}
		
	}
}
void vPotTASK(void)
{
	int i, pot_average;
	struct Message *pot_value;
	pot_value->id = 0; //0 for POT
	pot_value = &(mess);

	portTickType xLastWakeTime;
	const portTickType xFrequency = delay;

	
	
	while(1)
	{
		
		//Create an average value
		for(i = 1; i <= 50; i++) // Collecting 50 samples
		{
			adc_start(&AVR32_ADC);
			pot_value->pot = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL); //Read potentiometer value
			pot_average = pot_average + pot_value->pot;
		}
		
		//Calculate the average of 50 samples
		pot_average = pot_average / i;
		pot_value->pot = pot_average;
		pot_average=0;

		xQueueSend(xQueue, ( void * )&pot_value, (portTickType) 20); //Send the sensor value to main task
		
		//Delay 10ms
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		
	}
}
void vTmpTASK(void)
{
	int i, tmp_average;
	struct Message *tmp_value;
	tmp_value->id = 1; //1 for TMP
	tmp_value = &(mess);

	portTickType xLastWakeTime;
	const portTickType xFrequency = delay;
	
	
	
	while(1)
	{
		//Create an average value
		for(i = 1; i <= 50; i++) // Collecting 50 samples
		{
			adc_start(&AVR32_ADC);
			tmp_value->tmp = adc_get_value(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL); //Read potentiometer value
			tmp_average = tmp_average + tmp_value->tmp;
		}
		
		//Calculate the average of 50 samples
		tmp_average = tmp_average / i;
		tmp_value->tmp = tmp_average;
		tmp_average=0;
		

		xQueueSend(xQueue, ( void * )&tmp_value, (portTickType) 20); //Send the sensor value to main task
		
		//Delay 10ms
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
}
void vLdrTASK(void) 
{
	int i, ldr_average;
	struct Message *ldr_value;
	ldr_value->id = 2; //2 for LDR
	ldr_value = & (mess);
	
	portTickType xLastWakeTime;
	const portTickType xFrequency = delay;
	
	
	
	while(1)
	{
		//Create an average value
		for(i = 1; i <= 50; i++) // Collecting 50 samples
		{
			adc_start(&AVR32_ADC);
			ldr_value->ldr = adc_get_value(&AVR32_ADC, ADC_LIGHT_CHANNEL); //Read potentiometer value
			ldr_average = ldr_average + ldr_value->ldr;
		}
		
		//Calculate the average of 50 samples
		ldr_average = ldr_average / i;
		ldr_value->ldr = ldr_average;
		ldr_average=0;

		xQueueSend(xQueue, ( void * )&ldr_value, (portTickType) 20); //Send the sensor value to main task
		
		//Delay 10ms
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
	}
	
}