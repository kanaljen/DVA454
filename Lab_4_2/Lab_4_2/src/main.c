#include "includes.h"

int main (void)
{
	void LED0 init(void) {
		/* Select the pin we are going to use as GPIO */
		GPIO_PORT1_GPER |= (0x01 << 27);
		/* Enable the corresponding diver for that pin */
		GPIO_PORT1_ODER |= (0x01 << 27);
		/* Set the value of that pin to HIGH */
		GPIO_PORT1_OVR |= (0x01 << 27);
	}
	
	void LED0 init(void) {
		/* Select the pin we are going to use as GPIO */
		GPIO_PORT1_GPER |= (0x01 << 27);
		/* Enable the corresponding diver for that pin */
		GPIO_PORT1_ODER |= (0x01 << 27);
		/* Set the value of that pin to HIGH */
		GPIO_PORT1_OVR |= (0x01 << 27);
	}
}

