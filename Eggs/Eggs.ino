const int Egg1Pin = 5;
const int Egg2Pin = 6;
const int EggLow = 32;
const int EggHighNominal = 224;
const int EggHighInitial = 48;

int Egg1State = 0;
int Egg2State = 0;
int Egg1Increment = 1; 
int Egg2Increment = 2; 
int EggHigh = EggHighInitial;

void setup() {
  pinMode(Egg1Pin, OUTPUT);
  analogWrite(Egg1Pin, 0);
  pinMode(Egg2Pin, OUTPUT);
  analogWrite(Egg2Pin, 0);
  Serial.begin(9600);
}

void raiseLimit() {
  if (++EggHigh > EggHighNominal) {
    EggHigh = EggHighInitial;
  }
}

void loop() {
  analogWrite(Egg1Pin,Egg1State);
  analogWrite(Egg2Pin,Egg2State);
  Egg1State += Egg1Increment;
  Egg2State += Egg2Increment;
  if (Egg1State > EggHigh) {
    Egg1State = EggHigh;
    Egg1Increment = -Egg1Increment;
    raiseLimit();
  } else if (Egg1State < EggLow) {
    Egg1State = EggLow;
    Egg1Increment = -Egg1Increment;
    raiseLimit();
  }
  if (Egg2State > EggHigh) {
    Egg2State = EggLow;
    raiseLimit();
  }
  delay(190);
}
