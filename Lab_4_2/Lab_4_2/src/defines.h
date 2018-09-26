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
#define GPIO_GPER_OFFSET 0x00
#define GPIO_GPERS_OFFSET 0x04
#define GPIO_GPERC_OFFSET 0x08
#define GPIO_GPERT_OFFSET 0x0C

#define GPIO_ODER_OFFSET 0x40
#define GPIO_ODERS_OFFSET 0x44
#define GPIO_ODERC_OFFSET 0x48
#define GPIO_ODERT_OFFSET 0x4C

#define GPIO_OVR_OFFSET 0x50
#define GPIO_OVRS_OFFSET 0x54
#define GPIO_OVRC_OFFSET 0x58
#define GPIO_OVRT_OFFSET 0x5C

/* Some defines so we can access the GPIO port 1 registers directly */
#define GPIO_PORT1_GPER (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_GPER_OFFSET)))
#define GPIO_PORT1_GPERS (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_GPERS_OFFSET)))
#define GPIO_PORT1_GPERC (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_GPERC_OFFSET)))
#define GPIO_PORT1_GPERT (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_GPERT_OFFSET)))

#define GPIO_PORT1_ODER (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_ODER_OFFSET)))
#define GPIO_PORT1_ODERS (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_ODERS_OFFSET)))
#define GPIO_PORT1_ODERC (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_ODERC_OFFSET)))
#define GPIO_PORT1_ODERT (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_ODERT_OFFSET)))

#define GPIO_PORT1_OVR (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_OVR_OFFSET)))
#define GPIO_PORT1_OVRS (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_OVRS_OFFSET)))
#define GPIO_PORT1_OVRC (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_OVRC_OFFSET)))
#define GPIO_PORT1_OVRT (*((volatile uint32_t*)(GPIO_PORT1_ADDRESS + GPIO_OVRT_OFFSET)))

/* --------------------------- Power Manager ------------------------   */

/* Absolute PM BASE ADDRESS */
#define PM_BASE_ADRESS 0xFFFF0C00

#define PM_MCCTRL_OFFSET 0x0000 //Main clock control
#define PM_OSCCTRL0_OFFSET 0x0028 //Oscillator 0 control

#define PM_MCCTRL (*((volatile uint32_t*)(PM_BASE_ADRESS + PM_MCCTRL_OFFSET)))
#define PM_OSCCTRL0 (*((volatile uint32_t*)(PM_BASE_ADRESS + PM_OSCCTRL0_OFFSET)))

/* ----------------------- Timer/Counter ----------------------- */

#define TC_BASE_ADRESS 0xFFFF3800
#define TC_CHANNEL_OFFSET 0x00
#define TC_CHANNEL_ADRESS (TC_BASE_ADRESS + TC_CHANNEL_OFFSET)

#define TC_CCR_OFFSET 0x00
#define TC_CMR_OFFSET 0x04
#define TC_CV_OFFSET 0x10
#define TC_RC_OFFSET 0x1C

#define TC_CCR (*((volatile uint32_t*)(TC_CHANNEL_ADRESS + TC_CCR_OFFSET)))
#define TC_CMR (*((volatile uint32_t*)(TC_CHANNEL_ADRESS + TC_CMR_OFFSET)))
#define TC_CV (*((volatile uint32_t*)(TC_CHANNEL_ADRESS + TC_CV_OFFSET)))
#define TC_RC (*((volatile uint32_t*)(TC_CHANNEL_ADRESS + TC_RC_OFFSET)))



#endif 