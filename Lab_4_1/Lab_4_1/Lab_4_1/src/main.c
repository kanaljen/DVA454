#include "asf.h"
#include "functions.h"

int main (void)
{
	int pot_value = 0;
	int brightness_LED5 = 0;
	// Configure the ADC module and enable the 
	// potentiometer channel 
	adc_configure(&AVR32_ADC); 
	adc_enable(&AVR32_ADC , ADC_POTENTIOMETER_CHANNEL);
	
	
	while(true) { 
		
		// Start a ADC sampling of all active channels 
		adc_start(&AVR32_ADC);
		
		// Get the potentiometer value 
		pot_value = adc_get_value(&AVR32_ADC , ADC_POTENTIOMETER_CHANNEL);
		
		// Convert the potentiometer value to a value 
		// between 0-255 
		brightness_LED5 = pot_value * 255 / 1024;
		
		// Set the intensity of LED5 
		LED_Set_Intensity(LED5 , brightness_LED5);
	}
	// Never return 
	while(true);

}
