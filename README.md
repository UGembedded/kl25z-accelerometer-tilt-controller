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

## Pin connections

MMA8451Q accelerometer	SCL — I²C clock	PTE24	I²C0, ALT5
MMA8451Q accelerometer	SDA — I²C data	PTE25	I²C0, ALT5
RGB LED	Red	PTB18	TPM2 channel 0, ALT3
RGB LED	Green	PTB19	TPM2 channel 1, ALT3
RGB LED	Blue	PTD1	TPM0 channel 1, ALT4
Serial output	UART0 TX — transmit	PTA2	UART0, ALT2
Serial input	UART0 RX — receive	PTA1	UART0, ALT2; unused by this code

## Repository structure

```text
kl25z-accelerometer-tilt-controller/
├── README.md
├── .gitignore
├── src/
│   ├── main.c                 # Accelerometer readings and LED control
│   ├── board.c                # Clock checks and timing
│   ├── i2c.c                  # Accelerometer communication
│   └── uart.c                 # Serial output
├── include/
│   ├── PWM.h                  # LED definitions
│   ├── board.h                # Board function declarations
│   ├── i2c.h                  # I²C function declarations
│   └── UART0_TXRX2.h          # UART function declarations
└── docs/
    └── images/                # Project demonstration photos
```



## pictures

1 Green	Near 90° from flat; Y-axis dominant

<img width="480" height="640" alt="7" src="https://github.com/user-attachments/assets/9c67de23-d595-4b4b-9d98-42ef12dfeca8" />

2 Blue	Near 0°; approximately flat

<img width="480" height="640" alt="8" src="https://github.com/user-attachments/assets/6b633068-b5d8-4d52-8bdb-ca8a01a0bc9e" />

3 Red	Near 90° from flat; X-axis dominant

<img width="480" height="640" alt="5" src="https://github.com/user-attachments/assets/4cf81d84-99a3-4439-891e-122e833cca7f" />

4 Red/pink	Near 90° from flat; X-axis dominant

<img width="480" height="640" alt="6" src="https://github.com/user-attachments/assets/f587a604-bd73-4d9a-8df9-fca5d3ddc193" />

5 purple	Intermediate tilt; 45° tilt

<img width="480" height="640" alt="1" src="https://github.com/user-attachments/assets/f21e82e9-9ef3-495d-b5b4-81734613be28" />

6 Purple	Intermediate tilt; 45° tilt

<img width="480" height="640" alt="2" src="https://github.com/user-attachments/assets/6700f306-08b5-4853-baae-2ae71aa55719" />

7 purple	Intermediate tilt; 45° tilt

<img width="480" height="640" alt="3" src="https://github.com/user-attachments/assets/784c6808-c6dd-4a7f-86a2-1fbd33392018" />

8 purple	Intermediate tilt; 45° tilt

<img width="480" height="640" alt="4" src="https://github.com/user-attachments/assets/1c8f1f18-7889-4be9-9e44-3ec0b5ca39e3" />

