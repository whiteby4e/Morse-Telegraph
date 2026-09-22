#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

#define RX_PIN 16
#define TX_PIN 17

#define ACTIVE_BUZZER 25

HardwareSerial MorseSerial(2);

// ========================================
// Morse A-Z
// ========================================

const char* morseCodes[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",
  "....", "..", ".---", "-.-", ".-..", "--", "-.",
  "---", ".--.", "--.-", ".-.", "...", "-", "..-",
  "...-", ".--", "-..-", "-.--", "--.."
};

const char letters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// ========================================
// Variables
// ========================================

String currentMorse = "";
String message = "";

// ========================================
// Decode Morse
// ========================================

char decodeMorse(String code) {

  for (int i = 0; i < 26; i++) {

    if (code == morseCodes[i]) {
      return letters[i];
    }
  }

  return '?';
}

// ========================================
// Active Buzzer
// LOW = ON
// HIGH = OFF
// ========================================

void beep(int duration) {

  digitalWrite(ACTIVE_BUZZER, LOW);

  delay(duration);

  digitalWrite(ACTIVE_BUZZER, HIGH);
}

// ========================================
// TFT
// ========================================

void drawScreen() {

  tft.fillScreen(TFT_BLACK);

  // Title
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(5, 5);
  tft.println("MORSE");

  // Separator line
  tft.drawLine(0, 28, 128, 28, TFT_WHITE);

  // ====================================
  // Current Morse
  // ====================================

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(5, 38);

  if (currentMorse.length() > 0) {

    tft.println(currentMorse);

  } else {

    tft.println("_");
  }

  // ====================================
  // Message
  // ====================================

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(5, 70);

  tft.println("MESSAGE:");

  tft.setTextSize(2);
  tft.setCursor(5, 85);

  // Show only the last part of the message
  String shownMessage = message;

  if (shownMessage.length() > 10) {

    shownMessage =
      shownMessage.substring(shownMessage.length() - 10);
  }

  tft.println(shownMessage);
}

// ========================================
// SETUP
// ========================================

void setup() {

  // --------------------------
  // Active buzzer
  // --------------------------

  pinMode(ACTIVE_BUZZER, OUTPUT);

  // Low-Level Trigger
  // HIGH = OFF
  digitalWrite(ACTIVE_BUZZER, HIGH);

  // --------------------------
  // Serial Monitor
  // --------------------------

  Serial.begin(115200);

  // --------------------------
  // UART
  // Pico TX -> ESP32 GPIO16
  // --------------------------

  MorseSerial.begin(
    9600,
    SERIAL_8N1,
    RX_PIN,
    TX_PIN
  );

  // --------------------------
  // TFT
  // --------------------------

  tft.init();

  tft.setRotation(0);

  // Initial screen
  drawScreen();

  Serial.println();
  Serial.println("======================");
  Serial.println(" MORSE ESP32 RECEIVER");
  Serial.println("======================");
  Serial.println("Ready!");
}

// ========================================
// LOOP
// ========================================

void loop() {

  while (MorseSerial.available()) {

    char c = MorseSerial.read();

    // ==================================
    // DOT
    // ==================================

    if (c == '.') {

      currentMorse += '.';

      Serial.println("DOT");

      beep(80);

      drawScreen();
    }

    // ==================================
    // DASH
    // ==================================

    else if (c == '-') {

      currentMorse += '-';

      Serial.println("DASH");

      beep(150);

      drawScreen();
    }

    // ==================================
    // ENTER
    // Finish letter
    // ==================================

    else if (c == '\n') {

      Serial.println("ENTER");

      if (currentMorse.length() > 0) {

        char decoded =
          decodeMorse(currentMorse);

        message += decoded;

        Serial.print("LETTER: ");
        Serial.println(decoded);

        currentMorse = "";

        // Letter-end beep
        beep(200);

        drawScreen();
      }
    }

    // ==================================
    // SPACE
    // Word separator
    // ==================================

    else if (c == ' ') {

      Serial.println("SPACE");

      // If Morse is still pending,
      // decode it first.

      if (currentMorse.length() > 0) {

        char decoded =
          decodeMorse(currentMorse);

        message += decoded;

        currentMorse = "";
      }

      // Add a space
      message += ' ';

      // Short beep
      beep(80);

      drawScreen();
    }
  }
}
