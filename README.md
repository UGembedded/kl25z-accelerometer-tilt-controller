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

## Demonstration

The onboard RGB LED changes colour with board orientation:
**X → red**, **Y → green**, and **Z → blue**.

### Main orientations

| Green — Y-axis dominant | Blue — Z-axis dominant | Red/pink — X-axis dominant |
| :---: | :---: | :---: |
| <img src="https://github.com/user-attachments/assets/9c67de23-d595-4b4b-9d98-42ef12dfeca8" width="240" alt="Board held upright with the RGB LED glowing green"> | <img src="https://github.com/user-attachments/assets/6b633068-b5d8-4d52-8bdb-ca8a01a0bc9e" width="240" alt="Board approximately flat with the RGB LED glowing blue"> | <img src="https://github.com/user-attachments/assets/f587a604-bd73-4d9a-8df9-fca5d3ddc193" width="240" alt="Board held upright with the RGB LED glowing red-pink"> |
| Approximately upright (90°) | Approximately flat (0°) | Approximately upright (90°) |

### Intermediate orientations

Tilting the board between the main orientations mixes the LED colours.
The photos below show purple and pink shades, consistent with combined
red and blue output.

<p align="center">
  <img src="https://github.com/user-attachments/assets/f21e82e9-9ef3-495d-b5b4-81734613be28" width="220" alt="Tilted board with a pale purple LED">
  <img src="https://github.com/user-attachments/assets/6700f306-08b5-4853-baae-2ae71aa55719" width="220" alt="Tilted board with a purple LED">
</p>

<p align="center">
  <img src="https://github.com/user-attachments/assets/784c6808-c6dd-4a7f-86a2-1fbd33392018" width="220" alt="Tilted board with a pink-purple LED">
  <img src="https://github.com/user-attachments/assets/1c8f1f18-7889-4be9-9e44-3ec0b5ca39e3" width="220" alt="Tilted board with a pale purple-pink LED">
</p>

