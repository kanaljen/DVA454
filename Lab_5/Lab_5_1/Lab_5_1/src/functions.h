/*
 * functions.h
 *
 * Created: 2018-10-26 15:16:38
 *  Author: sbn14007
 */ 

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "gpio.h"
#include "pm.h"
#include "board.h"
#include "compiler.h"

#define STACK_SIZE configMINIMAL_STACK_SIZE

#define GPIO_MAX_PIN_NUMBER 32

//Define LED 0
#define LED0_PORT ( LED0_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED0_PIN ( LED0_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED0_BIT_VALUE (1 << LED0_PIN )

//Define LED 1
#define LED1_PORT ( LED1_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED1_PIN ( LED1_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED1_BIT_VALUE (1 << LED1_PIN )

//Define LED 2
#define LED2_PORT ( LED2_GPIO / GPIO_MAX_PIN_NUMBER )
#define LED2_PIN ( LED2_GPIO & ( GPIO_MAX_PIN_NUMBER -1))
#define LED2_BIT_VALUE (1 << LED2_PIN )

//Define button 0
#define BUTTON_PORT0 (GPIO_PUSH_BUTTON_0 >> 5)
#define BUTTON_PIN0 (1 << (GPIO_PUSH_BUTTON_0 & 0x1f))

//Define button 1
#define BUTTON_PORT1 (GPIO_PUSH_BUTTON_1 >> 5)
#define BUTTON_PIN1 (1 << (GPIO_PUSH_BUTTON_1 & 0x1f))

//Define button 1
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

void USART_init(void); // Initialize USART
void LED_init(void); // Initialize LEDS
void vBlinkLED0(void*); // Taskfunction for blinking led 0
void vBlinkLED1(void*); // Taskfunction for blinking led 1
void vBlinkLED2(void*); // Taskfunction for blinking led 2

#endif /* FUNCTIONS_H_ */