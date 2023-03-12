#ifndef ANALOG_INPUT_H
#define ANALOG_INPUT_H
#include <Pollable.h>

class AnalogInput: public Pollable {
public:
  AnalogInput();
  ~AnalogInput();
  virtual void poll();
};

#endif
