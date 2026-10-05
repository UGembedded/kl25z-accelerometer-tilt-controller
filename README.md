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
<img width="480" height="640" alt="1" src="https://github.com/user-attachments/assets/f21e82e9-9ef3-495d-b5b4-81734613be28" />
<img width="480" height="640" alt="2" src="https://github.com/user-attachments/assets/6700f306-08b5-4853-baae-2ae71aa55719" />
<img width="480" height="640" alt="3" src="https://github.com/user-attachments/assets/784c6808-c6dd-4a7f-86a2-1fbd33392018" />
<img width="480" height="640" alt="4" src="https://github.com/user-attachments/assets/1c8f1f18-7889-4be9-9e44-3ec0b5ca39e3" />
<img width="480" height="640" alt="5" src="https://github.com/user-attachments/assets/4cf81d84-99a3-4439-891e-122e833cca7f" />
<img width="480" height="640" alt="6" src="https://github.com/user-attachments/assets/f587a604-bd73-4d9a-8df9-fca5d3ddc193" />
<img width="480" height="640" alt="7" src="https://github.com/user-attachments/assets/9c67de23-d595-4b4b-9d98-42ef12dfeca8" />
<img width="480" height="640" alt="8" src="https://github.com/user-attachments/assets/6b633068-b5d8-4d52-8bdb-ca8a01a0bc9e" />




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
