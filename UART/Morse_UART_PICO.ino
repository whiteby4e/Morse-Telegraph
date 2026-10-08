/*
  Morse Telegraph - UART Version
  Pico transmitter -> ESP32 receiver.

  Pico GP0 (TX) -> ESP32 GPIO16 (RX)
  GND -> GND
*/

#include <Arduino.h>

constexpr uint8_t DOT_BUTTON = 14;
constexpr uint8_t DASH_BUTTON = 13;
constexpr uint8_t SPACE_BUTTON = 12;
constexpr uint8_t ENTER_BUTTON = 11;
constexpr uint8_t BUZZER = 15;

constexpr uint16_t DOT_TONE = 900;
constexpr uint16_t DASH_TONE = 600;
constexpr unsigned long DEBOUNCE_MS = 35;

struct Button {
  uint8_t pin;
  bool last;
  unsigned long changedAt;
};

Button buttons[] = {
  {DOT_BUTTON, HIGH, 0},
  {DASH_BUTTON, HIGH, 0},
  {SPACE_BUTTON, HIGH, 0},
  {ENTER_BUTTON, HIGH, 0},
};

struct Beep {
  bool active = false;
  unsigned long until = 0;
};

Beep beep;

void startBeep(uint16_t frequency, unsigned long duration) {
  tone(BUZZER, frequency);
  beep.active = true;
  beep.until = millis() + duration;
}

void serviceBeep() {
  if (beep.active && (long)(millis() - beep.until) >= 0) {
    noTone(BUZZER);
    beep.active = false;
  }
}

bool pressed(Button &button) {
  const bool current = digitalRead(button.pin);
  const unsigned long now = millis();

  if (current != button.last) {
    if (now - button.changedAt >= DEBOUNCE_MS) {
      button.last = current;
      button.changedAt = now;
      return current == LOW;
    }
  } else {
    button.changedAt = now;
  }
  return false;
}

void setup() {
  for (auto &button : buttons) {
    pinMode(button.pin, INPUT_PULLUP);
    button.last = digitalRead(button.pin);
    button.changedAt = millis();
  }

  pinMode(BUZZER, OUTPUT);
  Serial1.setTX(0);
  Serial1.setRX(1);
  Serial1.begin(9600);
  Serial.begin(115200);
  Serial.println("Morse Transmitter Ready");
}

void loop() {
  serviceBeep();

  if (pressed(buttons[0])) {
    Serial.println("DOT");
    Serial1.print(".");
    startBeep(DOT_TONE, 100);
  }

  if (pressed(buttons[1])) {
    Serial.println("DASH");
    Serial1.print("-");
    startBeep(DASH_TONE, 300);
  }

  if (pressed(buttons[2])) {
    Serial.println("SPACE");
    Serial1.print(" ");
    startBeep(400, 80);
  }

  if (pressed(buttons[3])) {
    Serial.println("ENTER");
    Serial1.print("\n");
    startBeep(1200, 150);
  }
}
