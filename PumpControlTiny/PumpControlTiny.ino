#include <avr/wdt.h>

const int ButtonPin = 4;
const int PowerPin = 2;
const int Blinker = 1;
const int Select = 0;
const unsigned long LongPress = 1000L;
const unsigned long MillisOn[] = {0,10L * 1000L,10L * 1000L,10L * 1000L};
unsigned long Limit = 0;
unsigned long ButtonLimit = 0;
unsigned long now = 1;
unsigned long lastFlip = 0;
int State = 0;
int PumpState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ButtonPin,INPUT_PULLUP);
  pinMode(PowerPin,OUTPUT);
  pinMode(Blinker,OUTPUT);
  pinMode(Select,OUTPUT);
  digitalWrite(PowerPin,HIGH);
  digitalWrite(Blinker,LOW);
  digitalWrite(Select,LOW);
  Limit = 0;
  wdt_enable(WDTO_8S);
}

void CheckButton (unsigned long now, int sensorVal) {
  switch (State) {
    case 0:
    if (LOW == sensorVal) {
      ButtonLimit = now + LongPress;
      State = 1;
    }
    break;
    case 1:
    if (LOW == sensorVal && now > ButtonLimit) {
      Limit = 0;
      State = 2;
      AbortPump();
    } else if (HIGH == sensorVal) {
      Limit = now + MillisOn;
      State = 0;
      StartPump(now);
    }
    wdt_reset();
    break;
    case 2:
    if (HIGH == sensorVal) {
      Limit = 0;
      State = 0;
    }
    wdt_reset();
    break;
  }
}

void StartPump(unsigned long now) {
  Limit = now + MillisOn[++PumpState];
}

void AbortPump() {
  Limit = -1;
  PumpState = 0;
}

void PumpControl(unsigned long now) {
  if (PumpState && now > Limit) {
    if (PumpState >= 3) {
      PumpState = 0;
      Limit = now;
    } else {
      Limit = now + MillisOn[++PumpState];
    }
  }
  switch (PumpState) {
    case 1:
      digitalWrite(PowerPin,LOW);
      digitalWrite(Select,LOW);
      wdt_reset();
      break;
    case 2:
      digitalWrite(PowerPin,LOW);
      digitalWrite(Select,HIGH);
      wdt_reset();
      break;
    case 3:
      digitalWrite(PowerPin,LOW);
      digitalWrite(Select,LOW);
      wdt_reset();
      break;
    default:
      digitalWrite(PowerPin,HIGH);
      digitalWrite(Select,LOW);
      Limit = 0;
      PumpState = 0;
      if (now < 100000000L) {
        wdt_reset();
      }
      break;
  }
}

void loop() {
  unsigned long now = millis();
  int sensorVal = digitalRead(ButtonPin);
  CheckButton(now, sensorVal);
  PumpControl(now);
  if (PumpState) {
    if (lastFlip + 1000 / PumpState < now) {
      if (HIGH == digitalRead(Blinker)) {
        digitalWrite(Blinker, LOW);
      } else {
        digitalWrite(Blinker, HIGH);
      }
      lastFlip = now;
    }
  } else {
    digitalWrite(Blinker, LOW);
  }
  delay(40);
}
