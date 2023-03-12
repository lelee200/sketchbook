#include <avr/wdt.h>

const int ButtonPin = 2;
const int PowerPin = 3;
//unsigned long TimeOn = ;
const unsigned long LongPress = 1000;
unsigned long Limit = 0;
unsigned long ButtonLimit = 0;
unsigned long now = 1;
int State = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ButtonPin,INPUT_PULLUP);
  pinMode(PowerPin,OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(PowerPin,HIGH);
  digitalWrite(13,LOW);
  Limit = 0;
  Serial.begin(9600);
  wdt_enable(WDTO_8S);
}

void longPress() {
  Limit = 0;
}

void shortPress() {
  Limit = now + 1000L * 120L;
}

void pollButton() {
  int sensorVal = digitalRead(ButtonPin);
  digitalWrite(13,sensorVal);
  unsigned long now = millis();
  switch (State) {
    case 0:
    if (LOW == sensorVal) {
      ButtonLimit = now + LongPress;
      State = 1;
    }
    break;
    case 1:
    if (LOW == sensorVal && now > ButtonLimit) {
      longPress();
      State = 2;
    } else if (HIGH == sensorVal) {
      shortPress();
      State = 0;
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

void checkTime() {
  if (now > Limit) {
    digitalWrite(PowerPin,HIGH);
    Limit = 0;
    if (now < 100000000L) {
      wdt_reset();
    }
  } else {
    digitalWrite(PowerPin,LOW);
    wdt_reset();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  pollButton();
  checkTime();
  delay(30);
}
