#include <avr/wdt.h>

const int ButtonPin = 0;
const int PowerPin = 1;
const int ButtonLED = 2;
const unsigned long LongPress = 1000;
unsigned long Limit = 0;
unsigned long ButtonLimit = 0;
unsigned long now = 1;
int State = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ButtonPin,INPUT_PULLUP);
  pinMode(PowerPin,OUTPUT);
  pinMode(ButtonLED,OUTPUT);
  digitalWrite(PowerPin,HIGH);
  digitalWrite(ButtonLED,LOW);
  Limit = 0;
//  Serial.begin(9600);
  wdt_enable(WDTO_8S);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorVal = digitalRead(ButtonPin);
  digitalWrite(ButtonLED,sensorVal);
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
      Limit = 0;
      State = 2;
    } else if (HIGH == sensorVal) {
      Limit = now + 1000L * 120L;
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
  delay(30);
}
