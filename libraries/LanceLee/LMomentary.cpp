#include <Arduino.h>
#include <LMomentary.h>

LMomentary::LMomentary() {
}

LMomentary::LMomentary(int inputPin) {
  init(inputPin);
}

LMomentary::~LMomentary() {
  return;
}

void LMomentary::init(int inputPin) {
  Pollable::init(inputPin,10);
  buttonState = 0;
  ignoreTime = 50;
  longThreshold = 1000;
}

void LMomentary::poll() {
  lastValue = digitalRead(inputPin)^inverse;
  switch (buttonState) {
    case 0:
      if (digitalRead(inputPin)^inverse) {
        buttonState = 1;
        lastStateChange = lastUpdateAt;
        onPress();
      }
    break;
    
    case 1:
      if (lastStateChange + ignoreTime < lastUpdateAt) {
        buttonState = 2;
        lastStateChange = lastUpdateAt;
      }
    break;
    
    case 2:
      if (!(digitalRead(inputPin)^inverse)) {
        buttonState = 4;
        lastStateChange = lastUpdateAt;
        onRelease();
        break;
      }
      if (lastStateChange + longThreshold < lastUpdateAt) {
        buttonState = 3;
        lastStateChange = lastUpdateAt;
        onLongPress();
      }
    break;
    
    case 3:
      if (!(digitalRead(inputPin)^inverse)) {
        buttonState = 4;
        lastStateChange = lastUpdateAt;
        onLongrelease();
      }
    break;
    
    case 4:
      if (lastStateChange + ignoreTime < lastUpdateAt) {
        buttonState = 0;
      }
    break;

    default:
    break;
  }
}

void LMomentary::setIgnoreTime(unsigned long ignoreTime) {
  this->ignoreTime = ignoreTime;
}

void LMomentary::setLongThreshold(unsigned long longThreshold) {
  this->longThreshold = longThreshold;
}

int LMomentary::getButtonState() {
  return buttonState;
}

void LMomentary::resetButtonState() {
  buttonState = 4;
}

void LMomentary::setInverse(int inverse) {
	this->inverse = inverse;
}


void LMomentary::onSample() {return;}

void LMomentary::onPress() {return;}

void LMomentary::onLongPress() {return;}

void LMomentary::onRelease() {return;}

void LMomentary::onLongrelease() {return;}

