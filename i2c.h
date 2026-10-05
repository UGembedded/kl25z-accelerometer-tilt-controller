#ifndef RGB_I2C_H
#define RGB_I2C_H
#include <stdint.h>
#include "board.h"
/* PTE24 SCL / PTE25 SDA, I2C0; unshifted 7-bit slave address. */
int i2c_init(void);
int i2c_WriteRegister(uint8_t address, uint8_t reg, uint8_t value);
uint8_t I2C_ReadRegister(uint8_t address, uint8_t reg);
extern volatile int I2C_LastError; /* Status of latest single-register read. */
int I2C_ReadMultiRegisters(uint8_t address, uint8_t first_reg,
                           uint8_t count, uint8_t *values);
#endif
