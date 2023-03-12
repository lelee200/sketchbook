#include <LMomentary.h>
#include <Pollable.h>
#include <Poller.h>

class LEDButton: public LMomentary {
  int ledIndex;
public:
  LEDButton(int buttonPin, int ledIndex): LMomentary(buttonPin) {
    init(buttonPin, ledIndex);
  };
  void init(int buttonPin, int ledIndex) {
    LMomentary::init (buttonPin);
    pinMode(ledIndex,OUTPUT);
    this->ledIndex = ledIndex;
  }
  
  void onPress () {
    digitalWrite(ledIndex,HIGH);
    Serial.println("press");
  };
  void onLongPress() {
     return;
  }
  void onRelease () {
    digitalWrite(ledIndex,LOW);
    Serial.println("release");
  };
  void onLongRelease() {
    return;
  }
};

Pollable *pb[2];
Poller poller(pb);

void setup() {
  Serial.begin(9600);
  pb[0] = new LEDButton(12,13);
  pb[1] = 0;
}

void loop() {
  poller.loop();
  delay(1);
}



