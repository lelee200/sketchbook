#include <Pollable.h>
#include <DigitalInput.h>
#include <Poller.h>

class DigitalTell: public DigitalInput {
  int prevValue;
public:
  DigitalTell() : DigitalInput() {
    prevValue = 0;
  }
  void onSample() {
    if (lastValue != prevValue) {  
      Serial.print("new value ");
      Serial.println(lastValue);
    }
    prevValue = lastValue;
  }
};

Pollable *pa[4];
Poller *pl;

void setup() {
  DigitalTell *at = new DigitalTell();
  at->init(12,10);
  pa[0] = at;
  pa[1] = 0;
  pl = new Poller(pa);
  Serial.begin(9600);
  Serial.println("hekko");
}

void loop () {
  pl->loop();
}
