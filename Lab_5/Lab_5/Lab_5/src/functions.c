#include "functions.h"

void USART_reset(volatile avr32_usart_t *usart)
{
	//Reset
	usart->CR.rstrx = 1;   //Resets the receiver (1)
	usart->CR.rsttx = 1;   //Resets the transmitter (1)
	usart->CR.rststa = 1;  //Resets status bits (1)
	usart->CR.rstnack = 1; //Reset non acknowledge (0)
	
	//"reset" the reset
	usart->CR.rstrx = 0;   //Resets the receiver (0)
	usart->CR.rsttx = 0;   //Resets the transmitter (0)
	usart->CR.rststa = 0;  //Resets status bits (0)
	usart->CR.rstnack = 0; //Reset non acknowledge (0)
	
	//re-enables receiver and transmitter
	usart->CR.rxdis = 0;   //Should be 0 to NOT disable receiver (0)
	usart->CR.rxen =  1;   //Enables receiver (1)
	usart->CR.txdis = 0;   //Should be 0 to NOT disable transmitter (0)
	usart->CR.txen = 1;    //Enables the transmitter (1)

}
void USART_init(volatile avr32_usart_t * usart)
{
	
	//Reset (Set all bits to 0 in the used registers)
	USART_reset(usart);
	
	//Control Register
	usart->CR.rxdis = 0;   //Should be 0 to NOT disable receiver (0)
	usart->CR.rxen =  1;   //Enables receiver (1)
	usart->CR.txdis = 0;   //Should be 0 to NOT disable transmitter (0)
	usart->CR.txen = 1;    //Enables the transmitter (1)
	usart->CR.sttbrk = 0;  //Start Break
	usart->CR.stpbrk = 0;  //Stop Break
	usart->CR.sttto = 0;   //Start time-out
	usart->CR.senda = 0;   //Send address (In Multi drop mode only) (0)
	usart->CR.rstit = 0;   //Reset Iterations (ISO7816 must be enabled) (0)
	usart->CR.retto = 0;   //Restart time-out (0)
	usart->CR.rtsen = 0;   //Request to send Enable 0
	usart->CR.rtsdis = 0;  //Request to send Disable 0
	
	//Mode register MR
	usart->MR.mode = 0;          //Mode, normal 0000
	usart->MR.usclks = 0;        //Clock selection, USART_CLK selected
	usart->MR.chrl = 3;          //Character length (8 bit)
	usart->MR.sync = 0;          //Synchronous mode select, async selected
	usart->MR.par = 4;           //Parity type (No parity)
	usart->MR.nbstop = 0;        //Number of stop bits (1 stop bit)
	usart->MR.chmode = 0;        //Channel mode (normal)
	usart->MR.msbf = 0;          //Bit order
	usart->MR.mode9 = 0;         //9-bit character length
	usart->MR.clko = 1;          //Clock output select (USART drives CLK pin)
	usart->MR.over = 0;          //Oversampling mode (16x)
	usart->MR.inack = 0;         //Inhibit non acknowledge (nack is generated)
	usart->MR.dsnack = 0;        //Disable successive NACK
	usart->MR.var_sync = 0;      //Variable synchronization of Command
	usart->MR.max_iteration = 0; //Max iterations in ISO7816 mode
	usart->MR.filter = 0;        //Infrared receiver line filter
	usart->MR.man = 0;           //Manchester encoder enable
	usart->MR.modsync = 0;       //Manchester synchronization mode
	usart->MR.onebit = 0;        //Start frame delimiter selector
	
	//Baud Rate Generator Register
	usart->BRGR.fp = 0;    // No fraction needed
	usart->BRGR.cd = 78;   // 12 000 000 / (8*9600(2 - 0)) = 78.125
	
	//Enable txrdy interrupt
	//usart->IER.txrdy = 1;
	
	//Transmit Holding Register
	usart->THR.txsynh = 0;   //The next char sent is encoded as a data (DATA SYNC)
	
	//Enable GPIO ports for communication
	volatile avr32_gpio_port_t *usart_gpio = &AVR32_GPIO.port[0];
	
	//Enable RXD pin in GPIO
	//usart_gpio->oderc = 1 << 5;  //Disable output drivers
	usart_gpio->gpers = 1 << 5;  //Make the GPIO control the pins
	usart_gpio->pmr0c = 1 << 5;  //Select peripheral A (clear) {pmr1, pmr0} = 00
	usart_gpio->pmr1c = 1 << 5;  //Select peripheral A (clear)
	usart_gpio->gperc = 1 << 5;  //Enable peripheral control
	
	//Enable TXD pin in GPIO
	//usart_gpio->oderc = 1 << 6;  //Disable output drivers
	usart_gpio->gpers = 1 << 6;  //Make the GPIO control the pins
	usart_gpio->pmr0c = 1 << 6;  //Select peripheral A (clear) {pmr1, pmr0} = 00
	usart_gpio->pmr1c = 1 << 6;  //Select peripheral A (clear)
	usart_gpio->gperc = 1 << 6;  //Enable peripheral control
	
	//Enable CLK pin in GPIO
	//usart_gpio->oderc = 1 << 7;  //Disable output drivers
	usart_gpio->gpers = 1 << 7;  //Make the GPIO control the pins
	usart_gpio->pmr0c = 1 << 7;  //Select peripheral A (clear) {pmr1, pmr0} = 00
	usart_gpio->pmr1c = 1 << 7;  //Select peripheral A (clear)
	usart_gpio->gperc = 1 << 7;  //Enable peripheral control
	
	volatile avr32_pm_t *usart_PM = &AVR32_PM;
	usart_PM->OSCCTRL0.mode = 4;    //Crystal is connected to xin/xout with gain G0
	usart_PM->OSCCTRL0.startup = 6; //Startup time 142 ms
	usart_PM->MCCTRL.osc0en = 1;    //Oscillator 0 ENABLED
	usart_PM->MCCTRL.osc1en = 0;    //Oscillator 1 DISABLED
	usart_PM->MCCTRL.mcsel = 1;     //Oscillator 0 is the source for the main clock
}
void USART_putStr(char *c)
{
	volatile avr32_usart_t *usart = &AVR32_USART1;
	int i;
	
	for(i = 0; i < 32; i++)
	{
		
		while(!usart->CSR.txrdy); //The transmitter reports to CSR txrdy = 1 which indicates that THR is empty and TXEMPTY is 0
		usart->THR.txchr = c[i]; //Receive whatever data is in the transmit holding register THR
		if(c[i] == '\0'){
			usart->THR.txchr = '\n';
			break;
		}
	}
}
void init_LED(void)
{
	volatile avr32_gpio_port_t * led0_port = &AVR32_GPIO.port[LED0_PORT];
	led0_port->gpers = LED0_BIT_VALUE; //Set Enable register
	led0_port->ovrs = LED0_BIT_VALUE; //Set output value register
	led0_port->oders = LED0_BIT_VALUE; //Set output drive register
	
	volatile avr32_gpio_port_t * led1_port = &AVR32_GPIO.port[LED1_PORT];
	led1_port->gpers = LED1_BIT_VALUE; //Set Enable register
	led1_port->ovrs = LED1_BIT_VALUE; //Set output value register
	led1_port->oders = LED1_BIT_VALUE; //Set output drive register
	
	volatile avr32_gpio_port_t * led2_port = &AVR32_GPIO.port[LED2_PORT];
	led2_port->gpers = LED2_BIT_VALUE; //Set Enable register
	led2_port->ovrs = LED2_BIT_VALUE; //Set output value register
	led2_port->oders = LED2_BIT_VALUE; //Set output drive register
}
void vLED_TASK0(void* pvParameters)
{
	char c[32] = "LED 0 toggled";
	volatile avr32_gpio_port_t * led0 = &AVR32_GPIO.port[LED0_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 1000;

	while(1)
	{
		xLastWakeTime = xTaskGetTickCount();
		led0->ovrt = LED0_BIT_VALUE;
		USART_putStr(c);
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		led0->ovrt = LED0_BIT_VALUE;
		USART_putStr(c);
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vLED_TASK1(void* pvParameters)
{
	char c[32] = "LED 1 toggled";
	volatile avr32_gpio_port_t * led1 = &AVR32_GPIO.port[LED1_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 2000;

	while(1)
	{
		
		led1->ovrt = LED1_BIT_VALUE;
		USART_putStr(c);
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		
		led1->ovrt = LED1_BIT_VALUE;
		USART_putStr(c);
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vLED_TASK2(void* pvParameters)
{
	char c[32] = "LED 2 toggled";
	volatile avr32_gpio_port_t * led2 = &AVR32_GPIO.port[LED2_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 3000;

	while(1)
	{
		xLastWakeTime = xTaskGetTickCount();
		led2->ovrt = LED2_BIT_VALUE;
		USART_putStr(c);
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
		led2->ovrt = LED2_BIT_VALUE;
		USART_putStr(c);
		xLastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}
void vbutton_TASK0(void* pvParameters)
{
	char c[32] = "Button 0 pressed";
	char c1[32] = "LED 0 toggled";
	volatile int button_state0; //Initialize button state 0
	volatile avr32_gpio_port_t * led0 = &AVR32_GPIO.port[LED0_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10000;
	xTaskHandle task  = *(xTaskHandle *)pvParameters;
	
	while(1){
		
		while(button_state0)
		button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0;
		
		if(!button_state0){
			USART_putStr(c);
			vTaskSuspend(task);
			led0->ovrc = LED0_BIT_VALUE;
			USART_putStr(c1);
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency);
			vTaskResume(task);
			
			while(!button_state0)
			button_state0 = AVR32_GPIO.port[BUTTON_PORT0].pvr & BUTTON_PIN0; //Read Button 1
		}
	}
}
void vbutton_TASK1(void* pvParameters)
{
	char c[32] = "Button 1 pressed";
	char c1[32] = "LED 1 toggled";
	volatile int button_state1; //Initialize button state 1
	volatile avr32_gpio_port_t * led1 = &AVR32_GPIO.port[LED1_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10000;
	xTaskHandle task  = *(xTaskHandle *)pvParameters;
	
	while(1){
		
		while(button_state1)
		button_state1 = AVR32_GPIO.port[BUTTON_PORT1].pvr & BUTTON_PIN1;
		
		if(!button_state1){
			USART_putStr(c);
			vTaskSuspend(task);
			led1->ovrc = LED1_BIT_VALUE;
			USART_putStr(c1);
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency);
			vTaskResume(task);
			
			while(!button_state1)
			button_state1 = AVR32_GPIO.port[BUTTON_PORT1].pvr & BUTTON_PIN1; //Read Button 1
		}
	}
}
void vbutton_TASK2(void* pvParameters)
{
	char c[32] = "Button 2 pressed";
	char c1[32] = "LED 2 toggled";
	volatile int button_state2; //Initialize button state 2
	volatile avr32_gpio_port_t * led2 = &AVR32_GPIO.port[LED2_PORT];
	portTickType xLastWakeTime;
	const portTickType xFrequency = 10000;
	xTaskHandle task = *(xTaskHandle *)pvParameters;

	while(1){
		
		while(button_state2)
		button_state2 = AVR32_GPIO.port[BUTTON_PORT2].pvr & BUTTON_PIN2; //Read Button 2
		
		if(!button_state2){
			USART_putStr(c);
			vTaskSuspend(task);
			led2->ovrc = LED2_BIT_VALUE;
			USART_putStr(c1);
			xLastWakeTime = xTaskGetTickCount();
			vTaskDelayUntil(&xLastWakeTime, xFrequency);
			vTaskResume(task);
			
			while(!button_state2)
			button_state2 = AVR32_GPIO.port[BUTTON_PORT2].pvr & BUTTON_PIN2; //Read Button 2
		}
	}
}