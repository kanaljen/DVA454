/*
 * led.h
 *
 * Created: 2018-10-25 16:16:12
 *  Author: sbn14007
 */ 
#include "stdio.h"

#ifndef LED_H_
#define LED_H_

#define TRUE 1
#define FALSE 0
#define LED_PIN 27

/* GPIO ADDRESSES AND OFFSETS */
#define GPIO_ADDRESS 0xFFFF1000  // Absolute address to General Purpose IO Controller, page 38
#define GPIO1_OFFSET 0x100  // Offset to Port 1 Configuration Registers, page 175
#define GPIO1_ADDRESS ( GPIO_ADDRESS + GPIO1_OFFSET )  // Absolute address to Port 1 Configuration Registers
#define GPIO_GPERS_OFFSET 0x04  // Offset to GPIO Enable Register, page 175
#define GPIO_ODERS_OFFSET 0x44  // Offset to Output Driver Enable Register, page 176
#define GPIO_OVRS_OFFSET 0x54  // Offset to Output Value Register (set), page 176
#define GPIO_OVRC_OFFSET 0x58  // Offset to Output Value Register (clear), page 176

/* POINTERS */
#define GPIO1_GPERS (*((volatile int*)(GPIO1_ADDRESS + GPIO_GPERS_OFFSET)))
#define GPIO1_ODERS (*((volatile int*)(GPIO1_ADDRESS + GPIO_ODERS_OFFSET)))
#define GPIO1_OVRS (*((volatile int*)(GPIO1_ADDRESS + GPIO_OVRS_OFFSET)))
#define GPIO1_OVRC (*((volatile int*)(GPIO1_ADDRESS + GPIO_OVRC_OFFSET)))

void LED_init(void);
void LED_set(int bool);

#endif /* LED_H_ */