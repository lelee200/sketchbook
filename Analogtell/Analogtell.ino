#include <Pollable.h>
#include <AnalogInput.h>
#include <Poller.h>

class AnalogTell: public AnalogInput {
  void onSample() {
    Serial.println(lastValue);
  }
};

Pollable *pa[4];
Poller *pl;

void setup() {
  AnalogTell *at = new AnalogTell();
  at->init(A0,10);
  pa[0] = at;
  pa[1] = 0;
  pl = new Poller(pa);
  Serial.begin(9600);
}

void loop () {
  pl->loop();
}
