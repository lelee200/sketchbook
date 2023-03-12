#ifndef POLLABLE_H
#define POLLABLE_H
#include <Arduino.h>

class Pollable {
protected:
  long lastUpdateAt;
  int inputPin;
  unsigned long sampleInterval;
  long lastValue;
  int suspended;
  
  Pollable();
  virtual ~Pollable();
  void setUpdate(long currentTimeMillis);
public:
  void init (int inputPin, unsigned long sampleInterval);
  long getUpdateAgeMillis(long currentTimeMillis);
  void perLoop(unsigned long currentTimeMillis);
  void suspend();
  void resume();
  int isSuspended();

  virtual void poll() = 0;
  virtual void onSample() = 0;
};
#endif
