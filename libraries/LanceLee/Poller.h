#ifndef POLLER_H
#define POLLER_H
#include <Pollable.h>

class Poller {
  Pollable **pollables;
public:
  Poller();
  Poller(Pollable **pollables);
  void loop();
};
#endif
