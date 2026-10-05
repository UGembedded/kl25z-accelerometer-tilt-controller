#ifndef RGB_BOARD_H
#define RGB_BOARD_H
#include <stdint.h>
/* All driver calls are blocking, foreground-only, with SysTick running. */
enum {
    APP_OK = 0, APP_TIMEOUT = -1, APP_NACK = -2,
    APP_ARBITRATION = -3, APP_ARGUMENT = -4, APP_CLOCK = -5,
    APP_SENSOR_ID = -6, APP_SENSOR_CONFIG = -7
};
int Board_Init(void);
uint32_t Board_Millis(void);
void Board_DelayMs(uint32_t milliseconds);
uint32_t Board_BusClockHz(void);
uint32_t Board_PeripheralClockHz(void);
void SysTick_Handler(void);
#endif
