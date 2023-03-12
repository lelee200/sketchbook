#include <Pollable.h>

Pollable::Pollable() {
  lastUpdateAt = 0;
}

Pollable::~Pollable() {return;};

void Pollable::perLoop(unsigned long currentTimeMillis) {
  if (getUpdateAgeMillis(currentTimeMillis) < sampleInterval || suspended) return;
  setUpdate(currentTimeMillis);
  poll();
  onSample();
}

void Pollable::init (int inputPin, unsigned long interval) {
  this->inputPin = inputPin;
  this->sampleInterval = interval;
  this->suspended = 0;
  pinMode(inputPin, INPUT);
}

void Pollable::setUpdate(long currentTimeMillis) {
  lastUpdateAt = currentTimeMillis;
}

long Pollable::getUpdateAgeMillis(long currentTimeMillis) {
  return currentTimeMillis - lastUpdateAt;
}

void Pollable::suspend() {
  suspended = 1;
}

void Pollable::resume() {
  suspended = 0;
}

int Pollable::isSuspended() {
  return suspended;
}
