#ifndef RGB_UART0_H
#define RGB_UART0_H
#include <stdint.h>
#include "board.h"
/* UART0: PTA2 TX, PTA1 RX. 57600 baud, 8 data bits, no parity, 1 stop bit. */
int UART0_init(void);
int sendStr(const char *text, int length);
int sendHelloWorld(void);
/* This application needs transmission only; reception is not enabled. */
#endif
