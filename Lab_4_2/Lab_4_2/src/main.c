#include "functions.h"
#include "defines.h"

int main (void)
{
	LED0_init();
	ext_clk_init();
	clk_mux();


	GPIO_PORT1_OVRT = (0x01 << 27);
	while(1);
	
}

