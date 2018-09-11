/*
 * Functions.h
 *
 * Created: 2018-09-11 13:26:40
 *  Author: wickt
 */ 


#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <functions.c>

void USART_init(volatile avr32_usart_t * usart);
char USART_getChar();
void USART_putChar(char c);
void USART_reset();

#endif /* FUNCTIONS_H_ */