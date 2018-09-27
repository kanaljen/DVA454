/********************************************************
 Name          : main.c
 Author        : Marcus Jansson
 Copyright     : GPL
 Description   : EVK1100 template
 **********************************************************/

// Include Files
#include "compiler.h"
#include "gpio.h"
#include "board.h"
#include "adc.h"
#include "display_init.h"
#include <stdio.h>

int main(void)
{
   int pot_value;
   int tmp_value;
   int ls_value;
   int pot_average = 0;
   int tmp_average = 0;
   int ls_average = 0;
   int i;
   int k = 0;
   const char ln1[16];
   const char ln2[16];
   const char ln3[16];
   int brightness_LED5;

   // Configure the ADC module and enable the potentiometer channel
   adc_configure(&AVR32_ADC);
   adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
   adc_enable(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL);
   adc_enable(&AVR32_ADC, ADC_LIGHT_CHANNEL);
   
   //Initiate display
   display_init();

   while(true)
   {
	   k++;
		// Start a ADC sampling of all active channels
		
		// Get the potentiometer, temperature and light sensor value
		for(i = 1; i <= 50; i++) // Collecting 50 samples
		{
			adc_start(&AVR32_ADC);

			pot_value = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL); //Read potentiometer value
			tmp_value = adc_get_value(&AVR32_ADC, ADC_TEMPERATURE_CHANNEL); //Read temperature value
			ls_value = adc_get_value(&AVR32_ADC, ADC_LIGHT_CHANNEL); //Read light sensor value
			pot_average = pot_average + pot_value; //sum all 50 potentiometer values
			tmp_average = tmp_average + tmp_value; //sum all 50 temperature value
			ls_average = ls_average + ls_value; //sum all 50 Read light sensor value
		}
		pot_average = pot_average / i; //Average of the 50 potentiometer samples
		tmp_average = tmp_average / i; //Average of the 50 temperature samples
		ls_average = ls_average / i; //Average of the 50 light sensor samples
		
		//Convert the values to strings
		sprintf(ln1, "POT: %d", pot_average);
		sprintf(ln2, "TMP: %d", tmp_average);
		sprintf(ln3, "LDR: %d", ls_average);
		
		//Reset samples
		pot_average = 0; 
		tmp_average = 0;
		ls_average = 0;
		
		// Convert the potentiometer value to a value btwn 0-255
		brightness_LED5 = pot_value * 255 / 1024;

		// Set the intensity of the LED
		LED_Set_Intensity(LED5, brightness_LED5);
		
		//Display
		dip204_set_cursor_position(1, 1);
		dip204_printf_string(ln1);
		
		dip204_set_cursor_position(1, 2);
		dip204_printf_string(ln2);
		
		dip204_set_cursor_position(1, 3);
		dip204_printf_string(ln3);
		
		if(!(k % 20))
		{
		dip204_clear_display();
		k = 0;
		}
   }

   // Never return from main
   while(true);
}
