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
#define MAX_PORTS 32

/* GPIO ADDRESSES AND OFFSETS */
#define GPIO_ADDRESS 0xFFFF1000
#define GPIO1_OFFSET 0x100
#define GPIO1_ADDRESS ( GPIO_ADDRESS + GPIO1_OFFSET )
#define GPIO_GPERS_OFFSET 0x04
#define GPIO_ODERS_OFFSET 0x44
#define GPIO_OVRS_OFFSET 0x54
#define GPIO_OVRC_OFFSET 0x58

/* POINTERS */
#define GPIO1_GPERS (*((volatile uint32_t*)(GPIO1_ADDRESS + GPIO_GPERS_OFFSET)))
#define GPIO1_ODERS (*((volatile uint32_t*)(GPIO1_ADDRESS + GPIO_ODERS_OFFSET)))
#define GPIO1_OVRS (*((volatile uint32_t*)(GPIO1_ADDRESS + GPIO_OVRS_OFFSET)))
#define GPIO1_OVRC (*((volatile uint32_t*)(GPIO1_ADDRESS + GPIO_OVRC_OFFSET)))

void LED_init(void);
void LED_set(uint8_t bool);

#endif /* LED_H_ */