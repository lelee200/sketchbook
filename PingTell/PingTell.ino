#include <Pollable.h>
#include <Ping.h>
#include <Poller.h>

class PingTell: public Ping {
public:
  PingTell() : Ping() {
    setUnits(PING_MM);
  };
  virtual void onSample() {
    Serial.print("onSample ");
    Serial.println(lastValue);
  }
};

Pollable *pa[4];
Poller *pl;

void setup() {
  PingTell *at = new PingTell();
  at->init(12,1000);
  pa[0] = at;
  pa[1] = 0;
  pl = new Poller(pa);
  Serial.begin(9600);
}

void loop () {
  pl->loop();
}
