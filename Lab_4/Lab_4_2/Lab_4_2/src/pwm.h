/*
 * pwm.h
 *
 * Created: 2018-10-25 16:15:52
 *  Author: sbn14007
 */ 

#include "stdio.h"

#ifndef PWM_H_
#define PWM_H_

/* TC ADDRESSES AND OFFSETS */
#define TC_ADDRESS 0xFFFF3800 // Absolute address to Time/Counter, page 39
#define TC_CHA0_OFFSET 0x00  // Offset to time channel 0, page 654
#define TC_CHA0_ADDRESS (TC_ADDRESS + TC_CHA0_OFFSET) // Absolute address to time channel 0
#define TC_CCR_OFFSET 0x00  // Offset to Channel Control Register, page 654
#define TC_CMR_OFFSET 0x04  // Offset to Channel Mode Register, page 654
#define TC_CV_OFFSET 0x10  // Offset to Counter Value, page 654
#define TC_REGC_OFFSET 0x1C  // Offset to Register C, page 654

/* TC POINTERS */
#define TC_CCR_ADDRESS (*((volatile uint32_t*)(TC_CHA0_ADDRESS + TC_CCR_OFFSET))) // Absolute address to Channel Control Register
#define TC_CMR_ADDRESS (*((volatile uint32_t*)(TC_CHA0_ADDRESS + TC_CMR_OFFSET))) // Absolute address to Channel Mode Register
#define TC_CV_ADDRESS (*((volatile uint32_t*)(TC_CHA0_ADDRESS + TC_CV_OFFSET))) // Absolute address to Counter Value
#define TC_RC_ADDRESS (*((volatile uint32_t*)(TC_CHA0_ADDRESS + TC_REGC_OFFSET))) // Absolute address to Register C

/* PM ADDRESS AND OFFSETS */
#define PM_ADDRESS 0xFFFF0C00 // Absolute address to Power Manager, page 38
#define PM_MCCTRL_OFFSET 0x0000  // Offset to Main Clock Control, page 66
#define PM_OSCCTRL0_OFFSET 0x0028 // Offset to Oscillator 0 Control Register, page 66

/* PM POINTERS */
#define PM_MCCTRL_ADDRESS (*((volatile uint32_t*)(PM_ADDRESS + PM_MCCTRL_OFFSET))) // Absolute address to Main Clock Control
#define PM_OSCCTRL0_ADDRESS (*((volatile uint32_t*)(PM_ADDRESS + PM_OSCCTRL0_OFFSET))) // Absolute address to Oscillator 0 Control Register

void PWM_init(void);
void TC_init(void);
void PM_init(void);

#endif /* PWM_H_ */