# Morse Telegraph

A Morse telegraph system with UART communication, Serial Monitor output, and TFT display.

## Versions

This repository contains different versions of the project:

- **Serial Monitor** — Raspberry Pi Pico Morse input with Serial Monitor output.
- **TFT** — ESP32 Morse receiver with an ST7735 TFT display.
- **UART** — ESP32 UART communication test for the Pico-to-ESP32 link.

## Hardware

The current setup uses:

- Raspberry Pi Pico
- ESP32-WROOM-32
- ST7735 128x160 TFT display
- Passive buzzer
- Active buzzer
- Push buttons
- UART connection

## Project Structure

```
Morse-Telegraph/
├── README.md
├── LICENSE
├── Serial_Monitor/
│   └── Morse_Receiver_Serial.ino
├── TFT/
│   └── Morse_Receiver_TFT.ino
└── UART/
    └── Morse_UART_Test.ino
```

## UART Connection

Current Pico to ESP32 connection:

- Pico GP0 (TX) -> ESP32 GPIO16 (RX)
- Pico GND -> ESP32 GND
- Baud rate: 9600
- 8N1 serial format

The Pico RX pin is not required for the current one-way communication.

## Pico Controls

- DOT -> GP14
- DASH -> GP13
- SPACE -> GP12
- ENTER -> GP11
- Passive buzzer -> GP15

## ESP32 TFT

The TFT uses the following pins through TFT_eSPI:

- SCK -> GPIO18
- MOSI -> GPIO23
- CS -> GPIO5
- DC/RS -> GPIO2
- RST -> GPIO4

MISO is unused.

## ESP32 Buzzer

The active low-level-trigger buzzer is connected to GPIO25.

- LOW = ON
- HIGH = OFF

## Features

- Morse code input
- Morse code decoding
- UART communication
- Serial Monitor output
- TFT display interface
- Buzzer feedback
- Separate test versions for easier debugging

## Project Status

Experimental / learning project.

More features and hardware may be added in the future.

## License

MIT License
