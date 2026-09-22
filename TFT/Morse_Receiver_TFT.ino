/*
  Morse Telegraph - TFT Version
  Receiver: ESP32-WROOM-32
  Display: ST7735 128x160 via TFT_eSPI

  UART:
    Pico GP0 (TX) -> ESP32 GPIO16 (RX)
    GND -> GND
    Baud: 9600

  TFT_eSPI:
    SCK  -> GPIO18
    MOSI -> GPIO23
    CS   -> GPIO5
    DC   -> GPIO2
    RST  -> GPIO4
    MISO -> unused

  Active buzzer:
    GPIO25
    LOW = ON
    HIGH = OFF
*/

#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define RX_PIN 16
#define TX_PIN 17
#define ACTIVE_BUZZER 25

HardwareSerial MorseSerial(2);

const char* morseCodes[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
  "....", "..", ".---", "-.-", ".-..", "--", "-.",
  "---", ".--.", "--.-", ".-.", "...", "-", "..-",
  "...-", ".--", "-..-", "-.--", "--.."
};

const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

String currentMorse = "";
String message = "";

char decodeMorse(const String& code) {
  for (int i = 0; i < 26; i++) {
    if (code == morseCodes[i]) return letters[i];
  }
  return '?';
}

void beep(int duration) {
  digitalWrite(ACTIVE_BUZZER, LOW);
  delay(duration);
  digitalWrite(ACTIVE_BUZZER, HIGH);
}

void drawScreen() {
  tft.fillScreen(TFT_BLACK);

  // Vintage-style frame
  tft.fillRect(0, 0, 128, 18, 0x6A04);
  tft.setTextColor(0xD6B5, 0x6A04);
  tft.setTextSize(1);
  tft.setCursor(8, 5);
  tft.print("MORSE TELEGRAPH");

  // Current Morse area
  tft.fillRect(4, 23, 120, 34, 0xD8C2);
  tft.drawRect(4, 23, 120, 34, 0xAB84);

  tft.setTextColor(0x3329, 0xD8C2);
  tft.setTextSize(1);
  tft.setCursor(9, 28);
  tft.print("CODE");

  tft.setTextSize(2);
  tft.setCursor(9, 39);
  if (currentMorse.length() > 0)
    tft.print(currentMorse);
  else
    tft.print("_");

  // Green/patina separator
  tft.fillRect(0, 61, 128, 4, 0x3B4B);

  // Message paper
  tft.fillRect(4, 69, 120, 86, 0xD8C2);
  tft.drawRect(4, 69, 120, 86, 0xAB84);

  tft.setTextColor(0x3329, 0xD8C2);
  tft.setTextSize(1);
  tft.setCursor(9, 74);
  tft.print("MESSAGE");

  String shownMessage = message;
  if (shownMessage.length() > 18)
    shownMessage = shownMessage.substring(shownMessage.length() - 18);

  int textWidth = shownMessage.length() * 6;
  int x = (128 - textWidth) / 2;
  if (x < 8) x = 8;

  tft.setTextSize(1);
  tft.setCursor(x, 91);
  tft.print(shownMessage);
}

void setup() {
  pinMode(ACTIVE_BUZZER, OUTPUT);
  digitalWrite(ACTIVE_BUZZER, HIGH);

  Serial.begin(115200);
  MorseSerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  tft.init();
  tft.setRotation(0);

  drawScreen();

  Serial.println();
  Serial.println("==============================");
  Serial.println("       MORSE TFT RECEIVER");
  Serial.println("==============================");
  Serial.println("Ready!");
}

void loop() {
  while (MorseSerial.available()) {
    char c = MorseSerial.read();

    if (c == '.') {
      currentMorse += '.';
      Serial.println("DOT");
      beep(80);
      drawScreen();
    }
    else if (c == '-') {
      currentMorse += '-';
      Serial.println("DASH");
      beep(150);
      drawScreen();
    }
    else if (c == '\n') {
      Serial.println("ENTER");

      if (currentMorse.length() > 0) {
        char decoded = decodeMorse(currentMorse);
        message += decoded;

        Serial.print("LETTER: ");
        Serial.println(decoded);

        currentMorse = "";
        beep(200);
        drawScreen();
      }
    }
    else if (c == ' ') {
      Serial.println("SPACE");

      if (currentMorse.length() > 0) {
        char decoded = decodeMorse(currentMorse);
        message += decoded;
        currentMorse = "";
      }

      message += ' ';
      beep(80);
      drawScreen();
    }
  }
}
