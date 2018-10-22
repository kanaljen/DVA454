/********************************************************
 Name          : main.c
 Author        : Marcus Jansson
 Copyright     : GPL
 Description   : EVK1100 template
 **********************************************************/

// Include Files
#include "board.h"
#include "adc.h"
#include "display_init.h"

int main(void)
{
   int pot_value;
   int brightness_LED5;

   // Configure the ADC module and enable the potentiometer channel
   adc_configure(&AVR32_ADC);
   adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
   
   // INit the display
   display_init();
   dip204_set_cursor_position(1,1);
   dip204_write_string("Potentiometer:"); // 14 length
   dip204_set_cursor_position(1,2);
   dip204_write_string("Light Sensor:"); // 13 length
   dip204_set_cursor_position(1,3);
   dip204_write_string("Temperature:"); // 12 length
   dip204_set_cursor_position(16,1);
   dip204_write_string("15");
   dip204_set_cursor_position(15,2);
   dip204_write_string("15");
   dip204_set_cursor_position(14,3);
   dip204_write_string("15");
   

   while(true)
   {
		// Start a ADC sampling of all active channels
		adc_start(&AVR32_ADC);

		// Get the potentiometer value
		pot_value = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);

		// Convert the potentiometer value to a value btwn 0-255
		brightness_LED5 = pot_value * 255 / 1024;

		// Set the intensity of the LED
		LED_Set_Intensity(LED4, brightness_LED5);
   }

   // Never return from main
   while(true);
}
