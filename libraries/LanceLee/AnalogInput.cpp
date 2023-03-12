#include <AnalogInput.h>
#include <Arduino.h>

void AnalogInput::poll() {
	lastValue = analogRead(inputPin);
}

AnalogInput::AnalogInput() {
	;
}
AnalogInput::~AnalogInput() {
	;
}

