#include "avr32\usart_400.h"
#include "avr32\uc3a0512.h"
#include "FreeRTOS.h"
#include "task.h"
#include "board.h"
#include "compiler.h"

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

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

void USART_init(volatile avr32_usart_t * usart);
void init_LED(void);
void USART_putStr(char *c);
void USART_reset(volatile avr32_usart_t * usart);
void vLED_TASK0(void* pvParameters);
void vLED_TASK1(void* pvParameters);
void vLED_TASK2(void* pvParameters);
void vbutton_TASK0(void* pvParameters);
void vbutton_TASK1(void* pvParameters);
void vbutton_TASK2(void* pvParameters);

#endif /* FUNCTIONS_H_ */