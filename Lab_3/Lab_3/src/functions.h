#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include "compiler.h"
#include "board.h"
#include "tc.h"
#include "intc.h"
#include "gpio.h"
#include <stdio.h>

#define TRUE 1
#define FALSE 0

void TC_init(volatile avr32_tc_t * tc);
void USART_init(volatile avr32_usart_t * usart);
char USART_getChar();
void USART_putChar(char c);
void USART_putStr(char *c);
void USART_reset(volatile avr32_usart_t * usart);

#endif /* FUNCTIONS_H_ */
