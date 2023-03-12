#include <Ping.h>
#include <Arduino.h>

Ping::Ping() : Pollable() {}
Ping::~Ping() {}
void Ping::init (int inputPin, unsigned long sampleInterval) {
  this->inputPin = inputPin;
  this->sampleInterval = sampleInterval;
  pinMode(inputPin, OUTPUT);
  digitalWrite(inputPin, LOW);
}

void Ping::setUnits(int units) {
  switch (units) {
    case PING_SS:
      speed_num = 1;
      speed_denom = 2;
      break;
    case PING_CM:
      speed_num = 34;
      speed_denom = 20;
      break;
    case PING_MM:
      speed_num = 34;
      speed_denom = 200;
      break;
    case PING_IN:
      speed_num = 1356;
      speed_denom = 200000;
      break;
  }
}

void Ping::poll() {
  pinMode(inputPin, OUTPUT);
  digitalWrite(inputPin, LOW);
  delayMicroseconds(2);
  digitalWrite(inputPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(inputPin, LOW);
  pinMode(inputPin, INPUT);
  lastValue = speed_num * pulseIn(inputPin, HIGH) / speed_denom;
  onSample();
}
