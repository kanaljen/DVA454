#include "stdio.h"
#include "led.h"
#include "tc.h"

int main (void)
{
	LED_init();
	TC_init();
	while(TRUE);
}
