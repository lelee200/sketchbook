#include "pitches.h"

// notes in the melody:
typedef struct {
  int pitch;
  int duration;
} Note;

const int InterruptPin = 2;
const int Selector = 3;
const int Buzzer = 4;

const Note Silence[] = {
  {0,0}
};

const Note Cucaracha[] = {
  {NOTE_C5,4},
  {NOTE_C5,4},
  {NOTE_C5,4},
  {NOTE_F5,2},
  {NOTE_A6,2},
  {NOTE_C5,4},
  {NOTE_C5,4},
  {NOTE_C5,4},
  {NOTE_F5,2},
  {NOTE_A6,2},
  {0,3},
  {NOTE_F5,2},
  {NOTE_F5,4},
  {NOTE_E5,4},
  {NOTE_E5,4},
  {NOTE_D5,4},
  {NOTE_D5,4},
  {NOTE_C5,2},
  {0,0}
};

const Note Cookie[] = {
  {NOTE_E4,2},
  {NOTE_E4,4},
  {NOTE_D4,4},
  {NOTE_E4,4},
  {NOTE_C4,2},
  {0,5},
  {NOTE_A3,4},
  {NOTE_E4,4},
  {NOTE_D4,4},
  {NOTE_E4,4},
  {NOTE_D4,4},
  {NOTE_C4,1},
  {0,0}
};

volatile Note* NextNote = Silence;

void setup() {
  pinMode(Selector, INPUT_PULLUP);
  pinMode(InterruptPin, INPUT_PULLUP);
}

void loop() {
  PlayNextSound();
  CheckTrigger();
}

void CheckTrigger() {
  if (NextNote->duration) return;
  if (HIGH == digitalRead(InterruptPin)) return;
  NextNote = HIGH == digitalRead(Selector) ? Cookie : Cucaracha;
}

void PlayNextSound() {
  if (NextNote->duration) {
    int noteDuration = 1000/NextNote->duration;
    tone (Buzzer, NextNote->pitch, noteDuration);
    int pauseBetweenNotes = noteDuration * 0.70;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(Buzzer);
    NextNote++;
  } else {
    delay(10);
  }
}
