/*
 * tc.h
 *
 * Created: 2018-10-25 16:15:52
 *  Author: sbn14007
 */ 

#include "stdio.h"

#ifndef TC_H_
#define TC_H_

/* TC ADDRESSES AND OFFSETS */
#define TC_ADDRESS 0xFFFF3800 // Absolute address to Time/Counter, page 39
#define TC_CHA0_OFFSET 0x00  // Offset to time channel 0, page 654
#define TC_CHA0_ADDRESS (TC_ADDRESS + TC_CHA0_OFFSET) // Absolute address to time channel 0
#define TC_CCR_OFFSET 0x00  // Offset to Channel Control Register, page 654
#define TC_CMR_OFFSET 0x04  // Offset to Channel Mode Register, page 654
#define TC_CV_OFFSET 0x10  // Offset to Counter Value, page 654
#define TC_REGC_OFFSET 0x1C  // Offset to Register C, page 654

/* TC POINTERS */
#define TC_CCR_ADDRESS (*((volatile int*)(TC_CHA0_ADDRESS + TC_CCR_OFFSET))) // Absolute address to Channel Control Register
#define TC_CMR_ADDRESS (*((volatile int*)(TC_CHA0_ADDRESS + TC_CMR_OFFSET))) // Absolute address to Channel Mode Register
#define TC_CV_ADDRESS (*((volatile int*)(TC_CHA0_ADDRESS + TC_CV_OFFSET))) // Absolute address to Counter Value
#define TC_RC_ADDRESS (*((volatile int*)(TC_CHA0_ADDRESS + TC_REGC_OFFSET))) // Absolute address to Register C

void TC_init(void);
void TC_start(void);

#endif /* TC_H_ */