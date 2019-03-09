Sensor R40 board pins reverse-engineered
========================================

Board have following chips:

- nRF52832 SoC
- BMP280 ow BME280 (in my case)
- MX25R8035 SPI Flash
- AP3216C light sensor (I^2C)
- BMI160 3d Accel/Gyro (I^2C)
- RGB LED (GPIO)

- 2 buttons (GPIO)
- 1 power switch
- CR2032 battery


nRF52832 Pinout
---------------

| PIN | NAME | Function | Description |
|-----|------|----------|-------------|
|   1 | DEC1 | ROWER | |
|   2 | XL1  | LSE 32.768 kHz | |
|   3 | XL2  | LSE | |
|   4 | P0.2 | | |
|   5 | P0.3 | | |
|   6 | P0.4 | | |
|   7 | P0.5 | | |
|   8 | P0.6 | | |
|   9 | P0.7 | | |
|  10 | P0.8 | | |
|  11 | P0.9/NFC1 | | |
|  12 | P0.10/NFC2 | | |
|-----|------|----------|-------------|
|  13 | VDD | POWER | |
|  14 | P0.11 | | |
|  15 | P0.12 | | |
|  16 | P0.13 | | |
|  17 | P0.14 | | |
|  18 | P0.15 | | |
|  19 | P0.16 | | |
|  20 | P0.17 | | |
|  21 | P0.18 | | |
|  22 | P0.19 | | |
|  23 | P0.20 | | |
|  24 | P0.21 | | |
|-----|------|----------|-------------|
|  25 | SWDCLK | SWD | |
|  26 | SWDIO | SWD | |
|  27 | P0.22 | | |
|  28 | P0.23 | | |
|  29 | P0.24 | | |
|  30 | ANT | RF | |
|  31 | VSS | POWER | |
|  32 | DEC2 | POWER | |
|  33 | DEC3 | POWER | |
|  34 | XC1 | HSE 16 MHz | |
|  35 | XC2 | HSE |
|  36 | VDD | POWER | |
|-----|------|----------|-------------|
|  37 | P0.25 | | |
|  38 | P0.26 | | |
|  39 | P0.27 | | |
|  40 | P0.28 | | |
|  41 | P0.29 | | |
|  42 | P0.30 | | |
|  43 | P0.31 | | |
|  44 | NC | n/a | |
|  45 | VSS | POWER | |
|  46 | DEC4 | POWER | |
|  47 | DCC | POWER | |
|  48 | VDD | POWER | |
