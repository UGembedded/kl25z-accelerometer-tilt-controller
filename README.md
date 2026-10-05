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
<img width="480" height="640" alt="1" src="https://github.com/user-attachments/assets/e8825bd7-0319-40c7-b09f-052378fb28d8" />


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
