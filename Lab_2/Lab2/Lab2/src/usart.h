#include "avr32\usart_400.h"
#include "avr32\uc3a0512.h"
#include "evk1100.h"

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

void USART_init(volatile avr32_usart_t * usart);
char USART_getChar();
void USART_putChar(char c);
void USART_reset(volatile avr32_usart_t * usart);

#endif /* FUNCTIONS_H_ */