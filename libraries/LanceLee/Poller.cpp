#include <Pollable.h>
#include <Arduino.h>
#include <Poller.h>

Poller::Poller(Pollable **pollables) {
  this->pollables = pollables;
}

void Poller::loop() {
  if (!pollables) return;
  long now = millis();
  Pollable **currentPollable = pollables;
  while (*currentPollable) {
    (*currentPollable)->perLoop(now);
    ++currentPollable;
  }
}

