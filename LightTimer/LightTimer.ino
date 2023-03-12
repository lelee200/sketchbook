#include <Wire.h>
#include "RTClib.h"

// PWM pins are 11 blue and 10 red
int PinMap[] = {11,10};
typedef struct {
  int8_t lightIndex;
  int8_t on;
  int8_t off;
} light;

light lights[] = {
  {0,8,18},
  {1,6,9},
  {1,17 ,20},
  {-1,1,-1}
};

RTC_DS1307 RTC;
RTC_Millis RTCM;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  if (RTC.isrunning()) {
    RTCM.begin(RTC.now());
  } else {
    RTCM.adjust(DateTime(__DATE__,__TIME__));
  }
  for(int index = 0; index < sizeof (PinMap) / sizeof (int); index++) {
    pinMode(PinMap[index],OUTPUT);
    digitalWrite(PinMap[index],LOW);
  }
}

void loop() {
  DateTime now;
  int pinState[sizeof (PinMap) / sizeof (int)];
  if (RTC.isrunning()) {
     RTCM.adjust(RTC.now());
  } else {
    Serial.println("RTC is NOT running!");
  }
  now = RTCM.now();
  Serial.print("Hour is ");
  Serial.println(now.hour());
  for(int index = 0; index < sizeof (PinMap) / sizeof (int); index++) {
    pinState[index] = 0;
  }
  for(int index = 0; lights[index].lightIndex >= 0; index++) {
    pinState[lights[index].lightIndex] |= now.hour() >= lights[index].on && now.hour() < lights[index].off;
  }
  for(int index = 0; index < sizeof (PinMap) / sizeof (int); index++) {
    digitalWrite(PinMap[index], pinState[index]?HIGH:LOW);
  }
  delay(10000);
}
