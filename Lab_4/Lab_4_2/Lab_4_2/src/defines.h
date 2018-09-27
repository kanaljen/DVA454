#include <stdio.h>

#ifndef def_h
#define def_h

typedef unsigned long uint32_t;
typedef unsigned char uint8_t;
/* ----------------------- GPIO --------------------------- */

/* Absolute GPIO BASE ADDRESS */
#define GPIO_BASE_ADDRESS 0xFFFF1000

/*Define the GPIO PORT1 OFFSET*/
#define GPIO_PORT1_OFFSET 0x100

/* Absolute GPIO PORT1 ADDRESS */
#define GPIO_PORT1_ADDRESS ( GPIO_BASE_ADDRESS + GPIO_PORT1_OFFSET )

/* Relative register offsets */
#define GPIO_GPERS_OFFSET 0x04
#define GPIO_ODERS_OFFSET 0x44
#define GPIO_OVRS_OFFSET 0x54
#define GPIO_OVRC_OFFSET 0x58

/* Some defines so we can access the GPIO port 1 registers directly */
#define GPIO_PORT1_GPERS (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_GPERS_OFFSET)))
#define GPIO_PORT1_ODERS (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_ODERS_OFFSET)))
#define GPIO_PORT1_OVRS (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_OVRS_OFFSET)))
#define GPIO_PORT1_OVRC (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_OVRC_OFFSET)))

/* --------------------------- Power Manager ------------------------   */

/* Absolute PM BASE ADDRESS */
#define PM_BASE_ADRESS 0xFFFF0C00

/* PM register offsets */
#define PM_MCCTRL_OFFSET 0x0000 //Main clock control
#define PM_OSCCTRL0_OFFSET 0x0028 //Oscillator 0 control

#define PM_MCCTRL (*((volatile uint32_t*)(PM_BASE_ADRESS + PM_MCCTRL_OFFSET)))
#define PM_OSCCTRL0 (*((volatile uint32_t*)(PM_BASE_ADRESS + PM_OSCCTRL0_OFFSET)))

/* ----------------------- Timer/Counter ----------------------- */

/* TC base adress */
#define TC_BASE_ADRESS 0xFFFF3800

/* TC channel 0 offset */
#define TC_CHANNEL_OFFSET 0x00
#define TC_CHANNEL_ADRESS (TC_BASE_ADRESS + TC_CHANNEL_OFFSET)

/* TC register offsets */
#define TC_CCR_OFFSET 0x00
#define TC_CMR_OFFSET 0x04
#define TC_CV_OFFSET 0x10
#define TC_RC_OFFSET 0x1C

#define TC_CCR (*((volatile uint32_t*)(TC_CHANNEL_ADRESS + TC_CCR_OFFSET)))
#define TC_CMR (*((volatile uint32_t*)(TC_CHANNEL_ADRESS + TC_CMR_OFFSET)))
#define TC_CV (*((volatile uint32_t*)(TC_CHANNEL_ADRESS + TC_CV_OFFSET)))
#define TC_RC (*((volatile uint32_t*)(TC_CHANNEL_ADRESS + TC_RC_OFFSET)))



#endif 