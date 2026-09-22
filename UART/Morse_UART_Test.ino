/*
  Morse Telegraph - UART Test Version
  Receiver: ESP32-WROOM-32

  This version is for testing the Pico -> ESP32 UART link.
  It does not use the TFT.

  UART:
    Pico GP0 (TX) -> ESP32 GPIO16 (RX)
    GND -> GND
    Baud: 9600
*/

#include <Arduino.h>

#define RX_PIN 16
#define TX_PIN 17

HardwareSerial MorseSerial(2);

void setup() {
  Serial.begin(115200);
  MorseSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  Serial.println();
  Serial.println("==============================");
  Serial.println("      MORSE UART TEST");
  Serial.println("==============================");
  Serial.println("Waiting for UART data...");
  Serial.println();
}

void loop() {
  while (MorseSerial.available()) {
    char c = MorseSerial.read();

    Serial.print("RX: ");

    if (c == '.') {
      Serial.println("DOT (.)");
    }
    else if (c == '-') {
      Serial.println("DASH (-)");
    }
    else if (c == '\n') {
      Serial.println("ENTER");
    }
    else if (c == ' ') {
      Serial.println("SPACE");
    }
    else {
      Serial.print("0x");
      if ((uint8_t)c < 16) Serial.print('0');
      Serial.println((uint8_t)c, HEX);
    }
  }
}
