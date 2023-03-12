#ifndef DIGITAL_INPUT_H
#define DIGITAL_INPUT_H
#include <Pollable.h>

class DigitalInput: public Pollable {
  public:
  DigitalInput();
  ~DigitalInput();
  virtual void poll();
};

#endif
