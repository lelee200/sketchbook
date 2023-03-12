#ifndef PING_H
#define PING_H
#include <Pollable.h>

#define PING_SS 0
#define PING_CM 1
#define PING_MM 2
#define PING_IN 3

class Ping: public Pollable {
  unsigned long speed_num = 1;
  unsigned long speed_denom = 2;
public:
  Ping();
  ~Ping();
  void setUnits(int);
  void init (int inputPin, unsigned long sampleInterval);
  virtual void onSample() = 0;
  virtual void poll();
};

#endif
