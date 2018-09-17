#include "compiler.h"
#include "board.h"
#include "functions.h"

int tc_channel = 0;
int value = 14400;
tc_write_rc(&AVR32_TC, tc_channel, value);

int tc_write_rc(volatile avr32_tc_t * tc,
unsigned int channel,
unsigned short value);int tc_start(volatile avr32_tc_t * tc, unsigned int channel);