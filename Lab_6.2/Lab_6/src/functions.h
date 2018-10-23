#include "avr32\usart_400.h"
#include "avr32\uc3a0512.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "compiler.h"
#include "pm.h"
#include "usart.h"
#include "gpio.h"
#include "semphr.h"
#include "stdio.h"
#include "display_init.h"
#include "adc.h"
#include "dip204.h"
#include <stdio.h>

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define TRUE 1
#define FALSE 0
#define GPIO_MAX_PIN_NUMBER 32

//Define button 0
#define BUTTON_PORT0 (GPIO_PUSH_BUTTON_0 >> 5)
#define BUTTON_PIN0 (1 << (GPIO_PUSH_BUTTON_0 & 0x1f))

#define configDBG 1
#define configDBG_USART (&AVR32_USART1)
#define configDBG_USART_RX_PIN AVR32_USART1_RXD_0_0_PIN
#define configDBG_USART_RX_FUNCTION AVR32_USART1_RXD_0_0_FUNCTION
#define configDBG_USART_TX_PIN AVR32_USART1_TXD_0_0_PIN
#define configDBG_USART_TX_FUNCTION AVR32_USART1_TXD_0_0_FUNCTION
#define configDBG_USART_BAUDRATE 9600

#define serialPORT_USART (&AVR32_USART1)
#define serialPORT_USART_RX_PIN AVR32_USART1_RXD_0_0_PIN
#define serialPORT_USART_RX_FUNCTION AVR32_USART1_RXD_0_0_FUNCTION
#define serialPORT_USART_TX_PIN AVR32_USART1_TXD_0_0_PIN
#define serialPORT_USART_TX_FUNCTION AVR32_USART1_TXD_0_0_FUNCTION
#define serialPORT_USART_IRQ AVR32_USART1_IRQ
#define serialPORT_USART_BAUDRATE 9600

#define DIP204_WRITE_DATA 0xFA
#define buffer_size 10

void USART_init(void);
void vSemaphoreTask(void* pvParameters);
void vLCDTask(void* pvParameters);
void vButtonTASK(void* pvParameters);
void vCountTASK(void* pvParameters);
void vSemaphoreTask( void * pvParameters );
void vDisplayPrintMSG(void);
void vGetMSGTASK(void* pvParameters);

#endif /* FUNCTIONS_H_ */