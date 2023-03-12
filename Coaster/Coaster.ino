/* Smart Coaster
** Pins used
**  A0 - Temperature sensor
**  13 - Red LED
**  12 - Green LED
**  11 - Buzzer
**  10 - Increment/Set Threshold
*/

#include <EEPROM.h>
#define IGNORE 20
#define LONG_PRESS 1000
#define TEMP_INCREMENT 30 * NUM_READINGS
#define WAIT_SAVE (5 * 1000)
#define RADIUS 50

#define NOTE_C5  523
#define NOTE_E5  659

typedef struct {
  int pitch;
  int duration;
} Note;

const Note Melody[] = {
  {NOTE_C5,4},
  {NOTE_E5,4},
  {NOTE_C5,4},
  {NOTE_E5,4},
  {NOTE_C5,2},
  {NOTE_C5,2},
  {NOTE_C5,4},
  {NOTE_C5,8},
  {NOTE_C5,8},
  {NOTE_C5,8},
  {NOTE_C5,4},
  {NOTE_E5,4},
  {NOTE_C5,2},
  {0,0}
};

#define NUM_NOTES (sizeof(Melody)/sizeof(Note))
#define NUM_READINGS (sizeof (Readings)/sizeof (long))
#define PRESSED LOW
#define RELEASED HIGH

const int SensorPin = A0;
const int RedLed = 13;
const int GreenLed = 12;
const int Buzzer = 11;
const int Button = 10;

long LastSum = 0;
int Readings[] = {
  0,0,0,0,0};
int ReadingIndex = 0;
long Threshold = 0;
char ButtonState = 0;
char TempState = 0;
long LastStateChange = 0;
long SaveAt = 0;
int NoteIndex = NUM_NOTES;

void setup() {
  pinMode(RedLed, OUTPUT);
  digitalWrite(RedLed, LOW);
  pinMode(GreenLed, OUTPUT);
  digitalWrite(GreenLed, LOW);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);
  pinMode(Button, INPUT);
  ReadFlash();
}

long ReadTemp () {
  Readings[ReadingIndex] = analogRead(SensorPin);
  long sum = 0;
  ReadingIndex = (ReadingIndex + 1) % NUM_READINGS;
  for (long sumIndex = 0; sumIndex < NUM_READINGS; sumIndex++)
    sum += Readings[sumIndex];
  return sum;
}

void ButtonEdge () {
  long now = millis();
  switch (ButtonState) {
    case 0:
      if (digitalRead(Button) == PRESSED) {
        ButtonState = 1;
        LastStateChange = now;
        NoteIndex = NUM_NOTES;
      }
    break;
    
    case 1:
      if (LastStateChange + IGNORE < now) {
        ButtonState = 2;
        LastStateChange = now;
      }
    break;
    
    case 2:
      if (digitalRead(Button) == RELEASED) {
        ButtonState = 4;
        Threshold += TEMP_INCREMENT;
        SaveAt = now + WAIT_SAVE;
        break;
        LastStateChange = now;
      }
      if (LastStateChange + LONG_PRESS < now) {
        ButtonState = 3;
        digitalWrite(RedLed, HIGH);
        digitalWrite(GreenLed, HIGH);
        TempState = 0;
      }
    break;
    
    case 3:
      if (digitalRead(Button) == RELEASED) {
        ButtonState = 4;
        digitalWrite(RedLed, LOW);
        digitalWrite(GreenLed, LOW);
        TempState = 0;
        Threshold = ReadTemp();
        LastStateChange = now;
      }
    break;
    
    case 4:
      if (LastStateChange + IGNORE < now) {
        TempState = 0;
        ButtonState = 0;
        SaveAt = now + 3000;
      }
    break;

    default:
    break;
  }
}

void TempEdge () {
  long temp = ReadTemp();
  switch (TempState) {
    case 0:
      if (temp > Threshold + RADIUS) {
        TempState = 1;
        digitalWrite(RedLed, HIGH);
      }
    break;
    
    case 1:
      if (temp < Threshold) {
        TempState = 2;
        digitalWrite(RedLed, LOW);
        digitalWrite(GreenLed, HIGH);
        NoteIndex = 0;
      }
    break;
    
    case 2:
      if (temp < Threshold - RADIUS) {
        TempState = 0;
        digitalWrite(RedLed, LOW);
        digitalWrite(GreenLed, LOW);
        NoteIndex = NUM_NOTES;
      } else if (temp > Threshold + RADIUS) {
        TempState = 1;
        digitalWrite(RedLed, HIGH);
        digitalWrite(GreenLed, LOW);
        NoteIndex = NUM_NOTES;
      }
    break;
  }
}

void PlayNextSound() {
  if (Melody[NoteIndex].pitch) {
    int noteDuration = 1000/Melody[NoteIndex].duration;
    tone (Buzzer, Melody[NoteIndex++].pitch, noteDuration);
    int pauseBetweenNotes = noteDuration * 0.70;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(Buzzer);
  } else {
    delay(10);
  }
}

void WriteFlash () {
  if (!SaveAt || millis() < SaveAt)
    return;
  SaveAt = 0;
  long saveVal = Threshold;
  for (int count = 0; count < 4; count++) {
    EEPROM.write(count, saveVal & 255);
    saveVal >>= 8;
  }
}

void ReadFlash () {
  Threshold = 0;
  for (int count = 3; count >= 0; count--) {
    Threshold = (Threshold << 8) | (255 & EEPROM.read(count));
  }
}

void loop () {
  ButtonEdge();
  TempEdge();
  PlayNextSound();
  WriteFlash();
}

