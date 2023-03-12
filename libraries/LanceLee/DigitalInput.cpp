#include <DigitalInput.h>
#include <Arduino.h>

void DigitalInput::poll() {
  lastValue = digitalRead(inputPin);
}

DigitalInput::DigitalInput() {;}
DigitalInput::~DigitalInput() {;}

