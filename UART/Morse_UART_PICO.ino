/*
  Morse Telegraph - UART Version
  Receiver: ESP32-WROOM-32
  
  UART:
    Pico GP0 (TX) -> ESP32 GPIO16 (RX)
    GND -> GND
   
*/

#define DOT_BUTTON 14
#define DASH_BUTTON 13
#define SPACE_BUTTON 12
#define ENTER_BUTTON 11

#define BUZZER 15

#define DOT_TONE 900
#define DASH_TONE 600

bool lastDot = HIGH;
bool lastDash = HIGH;
bool lastSpace = HIGH;
bool lastEnter = HIGH;

void beepDot() {
  tone(BUZZER, DOT_TONE);
  delay(100);
  noTone(BUZZER);
}

void beepDash() {
  tone(BUZZER, DASH_TONE);
  delay(300);
  noTone(BUZZER);
}

void setup() {
  pinMode(DOT_BUTTON, INPUT_PULLUP);
  pinMode(DASH_BUTTON, INPUT_PULLUP);
  pinMode(SPACE_BUTTON, INPUT_PULLUP);
  pinMode(ENTER_BUTTON, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);

  Serial1.setTX(0);
  Serial1.setRX(1);
  Serial1.begin(9600);

  Serial.begin(115200);

  Serial.println("Morse Transmitter Ready");
}

void loop() {

  bool dot = digitalRead(DOT_BUTTON);
  bool dash = digitalRead(DASH_BUTTON);
  bool space = digitalRead(SPACE_BUTTON);
  bool enter = digitalRead(ENTER_BUTTON);

  // DOT
  if (lastDot == HIGH && dot == LOW) {
    Serial.println("DOT");
    Serial1.print(".");

    beepDot();

    delay(150);
  }

  // DASH
  if (lastDash == HIGH && dash == LOW) {
    Serial.println("DASH");
    Serial1.print("-");

    beepDash();

    delay(150);
  }

  // SPACE = space between words
  if (lastSpace == HIGH && space == LOW) {
    Serial.println("SPACE");

    Serial1.print(" ");

    tone(BUZZER, 400);
    delay(80);
    noTone(BUZZER);

    delay(150);
  }

  // ENTER = end of letter
  if (lastEnter == HIGH && enter == LOW) {
    Serial.println("ENTER");

    Serial1.print("\n");

    tone(BUZZER, 1200);
    delay(150);
    noTone(BUZZER);

    delay(150);
  }

  lastDot = dot;
  lastDash = dash;
  lastSpace = space;
  lastEnter = enter;
}
