
enum Beat {
  Stop,
  Wh,
  Hf,
  Qt,
  Ei,
  Sx
};

class ToneSpec {
public:
  ToneSpec(int count, long duration) { this->count = count; this->duration = duration; };
  int count;
  long duration;
  int playCount(Beat b) { return (count >> (b)) + (count >> (b+1)); }
  static long noPlayDuration(Beat b) { return 500000 >> b; }
};

const ToneSpec Keys[] = {
  {16,30581},
  {17,28868},
  {18,27248},
  {19,25707},
  {21,24272},
  {22,22904},
  {23,21626},
  {25,20408},
  {26,19260},
  {28,18182},
  {29,17159},
  {31,16197},
  {33,15291},
  {35,14430},
  {37,13620},
  {39,12857},
  {41,12136},
  {44,11455},
  {46,10811},
  {49,10204},
  {52,9632},
  {55,9091},
  {58,8581},
  {62,8098},
  {65,7644},
  {69,7215},
  {73,6810},
  {78,6428},
  {82,6067},
  {87,5727},
  {93,5405},
  {98,5102},
  {104,4816},
  {110,4545},
  {117,4290},
  {123,4050},
  {131,3822},
  {139,3608},
  {147,3405},
  {156,3214},
  {165,3034},
  {175,2864},
  {185,2703},
  {196,2551},
  {208,2408},
  {220,2273},
  {233,2145},
  {247,2025},
  {262,1911},
  {277,1804},
  {294,1703},
  {311,1607},
  {330,1517},
  {349,1432},
  {370,1351},
  {392,1276},
  {415,1204},
  {440,1136},
  {466,1073},
  {494,1012},
  {523,956},
  {554,902},
  {587,851},
  {622,804},
  {659,758},
  {698,716},
  {740,676},
  {784,638},
  {831,602},
  {880,568},
  {932,536},
  {988,506},
  {1047,478},
  {1109,451},
  {1175,426},
  {1245,402},
  {1319,379},
  {1397,358},
  {1480,338},
  {1568,319},
  {1661,301},
  {1760,284},
  {1865,268},
  {1976,253},
  {2093,239},
  {2217,225},
  {2349,213},
  {2489,201},
  {2637,190},
  {2794,179},
  {2960,169},
  {3136,159},
  {3322,150},
  {3520,142},
  {3729,134},
  {3951,127},
  {4186,119},
  {4435,113},
  {4699,106},
  {4978,100},
  {5274,95},
  {5588,89},
  {5920,84},
  {6272,80},
  {6645,75},
  {7040,71},
  {7459,67},
  {7902,63}
};

enum Tone {
  _Rest,
  _C0,
  _CS0,
  _D0,
  _DS0,
  _E0,
  _F0,
  _FS0,
  _G0,
  _GS0,
  _A0,
  _AS0,
  _B0,
  _C1,
  _CS1,
  _D1,
  _DS1,
  _E1,
  _F1,
  _FS1,
  _G1,
  _GS1,
  _A1,
  _AS1,
  _B1,
  _C2,
  _CS2,
  _D2,
  _DS2,
  _E2,
  _F2,
  _FS2,
  _G2,
  _GS2,
  _A2,
  _AS2,
  _B2,
  _C3,
  _CS3,
  _D3,
  _DS3,
  _E3,
  _F3,
  _FS3,
  _G3,
  _GS3,
  _A3,
  _AS3,
  _B3,
  _C4,
  _CS4,
  _D4,
  _DS4,
  _E4,
  _F4,
  _FS4,
  _G4,
  _GS4,
  _A4,
  _AS4,
  _B4,
  _C5,
  _CS5,
  _D5,
  _DS5,
  _E5,
  _F5,
  _FS5,
  _G5,
  _GS5,
  _A5,
  _AS5,
  _B5,
  _C6,
  _CS6,
  _D6,
  _DS6,
  _E6,
  _F6,
  _FS6,
  _G6,
  _GS6,
  _A6,
  _AS6,
  _B6,
  _C7,
  _CS7,
  _D7,
  _DS7,
  _E7,
  _F7,
  _FS7,
  _G7,
  _GS7,
  _A7,
  _AS7,
  _B7,
  _C8,
  _CS8,
  _D8,
  _DS8,
  _E8,
  _F8,
  _FS8,
  _G8,
  _GS8,
  _A8,
  _AS8,
  _B8
};

// notes in the melody:
class Note {
public:
  Note(Tone pitch, Beat fraction) { this->pitch = pitch; this->fraction = fraction; };
  Tone pitch;
  Beat fraction;
};

const int Indicator = 0;
const int InterruptPin = 2;
const int Selector = 3;
const int Buzzer = 4;

const Note Silence[] = {
  Note(_Rest,Stop)
};

const Note Cucaracha[] = {
  {_C5,Qt},
  {_C5,Qt},
  {_C5,Qt},
  {_F5,Hf}, 
  {_A6,Hf},
  {_C5,Qt},
  {_C5,Qt},
  {_C5,Qt},
  {_F5,Hf},
  {_A6,Hf},
  {_Rest,Hf},
  {_Rest,Qt},
  {_F5,Hf},
  {_F5,Qt},
  {_E5,Qt},
  {_E5,Qt},
  {_D5,Qt},
  {_D5,Qt},
  {_C5,Hf},
  {_Rest,Stop}
};

const Note Cookie[] = {
  {_E4,Hf},
  {_E4,Qt},
  {_D4,Qt},
  {_E4,Qt},
  {_C4,Hf},
  {_Rest,Ei},
  {_A3,Qt},
  {_E4,Qt},
  {_D4,Qt},
  {_E4,Qt},
  {_D4,Qt},
  {_C4,Wh},
  {_Rest,Stop}
};

volatile Note* NextNote = Cookie;

void setup() {
  pinMode(Selector, INPUT_PULLUP);
  pinMode(InterruptPin, INPUT_PULLUP);
  pinMode(Buzzer, OUTPUT);
  digitalWrite(Buzzer, LOW);
  pinMode(Indicator, OUTPUT);
  digitalWrite(Indicator, LOW);
}

void loop() {
  PlayNextSound();
  //CheckTrigger();
}

void CheckTrigger() {
  if (Stop != NextNote->fraction) return;
  if (HIGH == digitalRead(InterruptPin)) return;
  NextNote = HIGH == digitalRead(Selector) ? Cookie : Cucaracha;
}

void PlayNextSound() {
  if (Stop == NextNote->fraction) {
    digitalWrite(Indicator, LOW);
    delay(1000);
    NextNote = Cucaracha;
    return;
  }
  ToneSpec spec = Keys[NextNote->pitch];
  Beat b = NextNote->fraction;
digitalWrite(Indicator, HIGH);
delay(1000);
digitalWrite(Indicator, LOW);
delay(1000);
for (int i=0;i<1000;i++) delayMicroseconds(Keys[NextNote->pitch].duration);
  for (int count = spec.playCount(0); count > 0; count--) {
    delayMicroseconds(Keys[NextNote->pitch].duration);
    digitalWrite(Buzzer,HIGH);
    delayMicroseconds(Keys[NextNote->pitch].duration);
    digitalWrite(Buzzer,LOW);
  }
  delay(Keys[NextNote->pitch].noPlayDuration(b));
  NextNote++;
}
