/*
 * define.h
 *
 * Created: 2018-10-30 16:04:23
 *  Author: sbn14007
 */ 


#ifndef DEFINE_H_
#define DEFINE_H_

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

#define TRUE 1
#define FALSE 0
#define STACK_SIZE configMINIMAL_STACK_SIZE
#define GPIO_MAX_PIN_NUMBER 32
#define BUFFER_SIZE 10

/* LEDS */
#define LED0_PORT ( LED0_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED0_PIN ( LED0_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED0_BIT_VALUE (1 << LED0_PIN )
#define LED1_PORT ( LED1_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED1_PIN ( LED1_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED1_BIT_VALUE (1 << LED1_PIN )
#define LED2_PORT ( LED2_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED2_PIN ( LED2_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED2_BIT_VALUE (1 << LED2_PIN )

/* BUTTONS */
#define BUTTON_PORT0 (GPIO_PUSH_BUTTON_0 >> 5)
#define BUTTON_PIN0 (1 << (GPIO_PUSH_BUTTON_0 & 0x1f))
#define BUTTON_PORT1 (GPIO_PUSH_BUTTON_1 >> 5)
#define BUTTON_PIN1 (1 << (GPIO_PUSH_BUTTON_1 & 0x1f))
#define BUTTON_PORT2 (GPIO_PUSH_BUTTON_2 >> 5)
#define BUTTON_PIN2 (1 << (GPIO_PUSH_BUTTON_2 & 0x1f))

/* USART */
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

/* FUNCTIONS */
void USART_init(void);
void LED_init(void);
void vWriteLine(char* str);
void initSemaphore(void);
void tskProducer(void* ptr);
void tskConsumer(void* ptr);


#endif /* DEFINE_H_ */