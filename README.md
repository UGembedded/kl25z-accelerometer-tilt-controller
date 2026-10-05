# KL25Z Accelerometer Tilt Controller

Bare-metal embedded C project using the NXP FRDM-KL25Z and its
MMA8451Q 3-axis accelerometer to change the onboard RGB LED colour
according to board orientation.

## Hardware

- NXP FRDM-KL25Z
- MMA8451Q 3-axis accelerometer
- On-board RGB LED

## Features

- Bare-metal register-level programming
- I2C communication with the MMA8451Q
- 14-bit X/Y/Z acceleration acquisition
- Tilt/orientation detection
- PWM control of the RGB LED
- UART debugging output
- Basic communication timeout/error handling

## System Overview

MMA8451Q Accelerometer
        |
        | I2C
        v
     KL25Z MCU
        |
        +----> Tilt/orientation logic
        |
        +----> PWM ---> RGB LED
        |
        +----> UART ---> Serial debugging

## Source Files

- `main.c` - application logic, accelerometer configuration and LED control
- `board.c/.h` - MCU clock and timing configuration
- `i2c.c/.h` - I2C driver
- `uart.c` / `UART0_TXRX2.h` - UART communication
- `PWM.h` - PWM/LED interface

## Development Environment

- NXP FRDM-KL25Z
- ARM Cortex-M0+
- Embedded C
- CMSIS KL25Z device header (`MKL25Z4.h`)
