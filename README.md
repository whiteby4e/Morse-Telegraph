<div align="center">

# Morse Telegraph

**A hands-on Morse communication system built with microcontrollers.**

Explore button input, Morse decoding, UART communication, buzzer feedback, and a small TFT interface.

![Platform](https://img.shields.io/badge/Platform-Raspberry%20Pi%20Pico%20%7C%20ESP32-blue)
![License](https://img.shields.io/badge/License-MIT-green)

</div>

---

## 🧭 Project versions

This repository contains three related implementations:

| Folder | Purpose |
|---|---|
| [Serial_Monitor](Serial_Monitor/) | Pico Morse input with Serial Monitor output |
| [UART](UART/) | Pico transmitter to ESP32 receiver over UART |
| [TFT](TFT/) | ESP32 Morse receiver with an ST7735 display |

## ✨ Features

- Enter dots and dashes with physical buttons.
- Decode Morse letters on the receiver.
- Send input from a Raspberry Pi Pico to an ESP32 over UART.
- Display decoded text on an ST7735 TFT.
- Get audible feedback from a buzzer.
- Keep separate versions for easier experimentation and debugging.

## 🔌 Hardware

The UART + TFT setup uses:

- Raspberry Pi Pico
- ESP32-WROOM-32
- ST7735 128 × 160 TFT
- Push buttons
- Passive buzzer on the Pico transmitter
- Active low-level-trigger buzzer on the ESP32 receiver

## 🔗 UART wiring

| Raspberry Pi Pico | ESP32 |
|---|---|
| GP0 (UART TX) | GPIO16 (UART RX) |
| GND | GND |

UART settings: **9600 baud, 8 data bits, no parity, 1 stop bit (8N1)**.

The current transmitter sends data one way, so the Pico RX connection is not required.

## 🎛️ Pico button pins

| Input | Pico GPIO |
|---|---:|
| Dot | GP14 |
| Dash | GP13 |
| Space | GP12 |
| Enter / finish letter | GP11 |
| Passive buzzer | GP15 |

## 🖥️ ESP32 TFT pins

The TFT example uses TFT_eSPI with:

| Signal | ESP32 GPIO |
|---|---:|
| SCK | 18 |
| MOSI | 23 |
| CS | 5 |
| DC / RS | 2 |
| RST | 4 |

MISO is unused in this setup. Configure TFT_eSPI for your display before uploading.

## 🚀 Getting started

1. Open the sketch for the version you want to test.
2. Select the correct board and port in Arduino IDE.
3. For the UART version, connect Pico GP0 to ESP32 GPIO16 and connect grounds.
4. Upload the Pico transmitter sketch.
5. Upload the ESP32 receiver sketch.
6. Open the Serial Monitor at 115200 baud on the board being debugged.

The UART link itself runs at 9600 baud; the Serial Monitor uses 115200 baud.

## 📁 Repository layout

```text
Morse-Telegraph/
├── Serial_Monitor/
│   └── Morse_Receiver_Serial.ino
├── TFT/
│   └── Morse_Receiver_TFT.ino
└── UART/
    ├── Morse_UART_PICO.ino
    └── Morse_UART_ESP32.ino
```

## 🧪 Project status

Experimental learning project. The implementations are intended as a practical starting point for microcontroller communication and Morse-code experiments.

## 📄 License

MIT — see [LICENSE](LICENSE).
