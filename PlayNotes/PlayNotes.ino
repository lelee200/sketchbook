/*
  PlayNotes
*/

#include "pitches.h"

const int Buzzer = 11;

// notes in the melody:
typedef struct {
  int pitch;
  int duration;
} Note;

int NoteIndex = 0;

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

const Note Bus[] = {
  {NOTE_C4,2},
  
  {NOTE_F4,2},
  {NOTE_F4,4},
  {NOTE_F4,4},
  
  {NOTE_F4,2},
  {NOTE_A5,2},
  
  {NOTE_C5,2},
  {NOTE_A5,2},
  
  {NOTE_F4,1},
  
  {NOTE_G4,2},
  {NOTE_G4,2},
  {NOTE_G4,1},

  {NOTE_E4,2},
  {NOTE_D4,2},

  {NOTE_C4,2},
  {NOTE_C4,2},

  {NOTE_F4,2},
  {NOTE_F4,4},
  {NOTE_F4,4},

  {NOTE_F4,2},
  {NOTE_A5,2},

  {NOTE_C5,2},
  {NOTE_A5,2},

  {NOTE_F4,1},
  
  {NOTE_G4,1},

  {NOTE_C4,1},
  
  {NOTE_F4,1},

  {0,0}
};

const Note Beer[] = {
  {NOTE_G4,4},
  {NOTE_G4,4},
  {NOTE_G4,4},
  {NOTE_D4,4},
  {NOTE_D4,4},
  {NOTE_D4,4},

  {NOTE_G4,4},
  {NOTE_G4,4},
  {NOTE_G4,4},
  {NOTE_G4,2},

  {NOTE_A5,4},
  {NOTE_A5,4},
  {NOTE_A5,4},
  {NOTE_E4,4},
  {NOTE_E4,4},
  {NOTE_E4,4},

  {NOTE_A5,2},
  {NOTE_G4,2},

  {NOTE_F4,4},
  {NOTE_F4,4},
  {NOTE_F4,4},
  {NOTE_D4,4},
  {NOTE_D4,4},
  {NOTE_D4,4},

  {NOTE_F4,4},
  {NOTE_F4,4},
  {NOTE_F4,4}, 
  {NOTE_F4,4},
  {NOTE_F4,4},
  {NOTE_E4,4},

  {NOTE_D4,4},
  {NOTE_D4,4},
  {NOTE_D4,4},

  {NOTE_D4,4},
  {NOTE_E4,4},
  {NOTE_F4,4},

  {NOTE_G4,4},
  {NOTE_G4,4},
  {NOTE_G4,4},

  {NOTE_G4,2},
  {0,0}
};

const Note Shark[] = {
  {NOTE_D4,1},
  {NOTE_E4,1},
  
  {NOTE_G4,2},
  {NOTE_G4,2},
  {NOTE_G4,2},
  {NOTE_G4,4},
  {NOTE_G4,4},

  {NOTE_G4,4},
  {NOTE_G4,4},
  {NOTE_G4,2},
  {NOTE_D4,1},
  {NOTE_E4,1},
  
  {NOTE_G4,2},
  {NOTE_G4,2},
  {NOTE_G4,2},
  {NOTE_G4,4},
  {NOTE_G4,4},

  {NOTE_G4,4},
  {NOTE_G4,4},
  {NOTE_G4,2},

  {NOTE_G4,2},
  {NOTE_G4,2},

  {NOTE_E4,1},
  {0,0}
};

const Note *Melodies[] = {
  Shark,
  Beer,
  Bus,
  Cucaracha,
  Cookie,
}; 
const int numMelodies = (sizeof Melodies) / (sizeof (Note*));
int MelIndex = 0;
const Note *Melody = Melodies[0];

void setup() {
}

void loop() {
  PlayNextSound();
}

void PlayNextSound() {
  if (Melody[NoteIndex].duration) {
    int noteDuration = 1000/Melody[NoteIndex].duration;
    tone (Buzzer, Melody[NoteIndex++].pitch, noteDuration);
    int pauseBetweenNotes = noteDuration * 0.70;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(Buzzer);
  } else {
    delay(5000);
    NoteIndex = 0;
    Melody = Melodies[++MelIndex%numMelodies];
  }
}
