#define DOT_BUTTON   14
#define DASH_BUTTON  13
#define SPACE_BUTTON 12
#define ENTER_BUTTON 11

#define BUZZER 15

//The sound of the Morse code machine
#define TONE_FREQ 650

// Audio Timing
#define DOT_TIME  100
#define DASH_TIME 300

// Interval after the sound
#define GAP_TIME 100

String currentMorse = "";
String message = "";

// Morse Code Table A-Z
const char* morseTable[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.",
  "--.", "....", "..", ".---", "-.-", ".-..",
  "--", "-.", "---", ".--.", "--.-", ".-.",
  "...", "-", "..-", "...-", ".--", "-..-",
  "-.--", "--.."
};

void beep(int duration) {
  tone(BUZZER, TONE_FREQ);
  delay(duration);
  noTone(BUZZER);
  delay(GAP_TIME);
}

char decodeMorse(String code) {

  for (int i = 0; i < 26; i++) {
    if (code == morseTable[i]) {
      return 'A' + i;
    }
  }

  return '?';
}

void setup() {

  pinMode(DOT_BUTTON, INPUT_PULLUP);
  pinMode(DASH_BUTTON, INPUT_PULLUP);
  pinMode(SPACE_BUTTON, INPUT_PULLUP);
  pinMode(ENTER_BUTTON, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);

  Serial.begin(115200);

  Serial.println();
  Serial.println("===== MORSE MACHINE =====");
  Serial.println("DOT   = GP14");
  Serial.println("DASH  = GP13");
  Serial.println("SPACE = GP12");
  Serial.println("ENTER = GP11");
  Serial.println();
}

void loop() {

  // --------------------
  // DOT
  // --------------------
  if (digitalRead(DOT_BUTTON) == LOW) {

    beep(DOT_TIME);

    currentMorse += ".";

    Serial.print(".");
    
    while (digitalRead(DOT_BUTTON) == LOW) {
      delay(10);
    }

    delay(50);
  }


  // --------------------
  // DASH
  // --------------------
  if (digitalRead(DASH_BUTTON) == LOW) {

    beep(DASH_TIME);

    currentMorse += "-";

    Serial.print("-");

    while (digitalRead(DASH_BUTTON) == LOW) {
      delay(10);
    }

    delay(50);
  }


  // --------------------
  // SPACE = end the word
  // --------------------
  if (digitalRead(SPACE_BUTTON) == LOW) {

    if (currentMorse.length() > 0) {

      char letter = decodeMorse(currentMorse);

      message += letter;

      Serial.print(" -> ");
      Serial.println(letter);

      currentMorse = "";
    }

    while (digitalRead(SPACE_BUTTON) == LOW) {
      delay(10);
    }

    delay(100);
  }


  // --------------------
  // ENTER = end
  // --------------------
  if (digitalRead(ENTER_BUTTON) == LOW) {

    // If anything hasn't been converted yet,
    // convert the last character as well.
    if (currentMorse.length() > 0) {

      char letter = decodeMorse(currentMorse);

      message += letter;

      currentMorse = "";
    }

    Serial.println();
    Serial.println("====================");
    Serial.print("MESSAGE: ");
    Serial.println(message);
    Serial.println("====================");

    // next text
    message = "";

    while (digitalRead(ENTER_BUTTON) == LOW) {
      delay(10);
    }

    delay(200);
  }
}
