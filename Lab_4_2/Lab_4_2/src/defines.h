#ifndef def_h
#define def_h

#include <stdio.h>

#define PIN_PB_27 59
#define LED0_GPIO PIN_PB_27
//Define number of pins

#define LED0_GPIOPort ( LED0_GPIO / 32 )

/* Absolute GPIO BASE ADDRESS */
#define GPIO_BASE_ADDRESS 0xFFFF1000

/*Define the GPIO PORT1 OFFSET*/
#define GPIO_PORT1_OFFSET 0x100

/* Absolute GPIO PORT1 ADDRESS */
#define GPIO_PORT1_ADDRESS ( GPIO_BASE_ADDRESS + GPIO_PORT1_OFFSET )

/* Relative register offsets */
#define GPIO_GPER_OFFSET 0x00
#define GPIO_ODER_OFFSET 0x40
#define GPIO_OVR_OFFSET 0x50

/* Some defines so we can access the GPIO port 1 registers directly */
#define GPIO_PORT1_GPER (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_GPER_OFFSET)))
#define GPIO_PORT1_ODER (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_ODER_OFFSET)))
#define GPIO_PORT1_OVR (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_OVR_OFFSET)))

#endif def_h