#ifndef LMOMENTARY_H
#define LMOMENTARY_H
#include <Pollable.h>

class LMomentary: public Pollable {
  long unsigned ignoreTime;
  long unsigned longThreshold;
  long unsigned lastStateChange;
  int buttonState = 0;
  int inverse = 0;
public:
  LMomentary();
  LMomentary(int buttonPin);
  void setIgnoreTime(unsigned long ignoreTime);
  void setLongThreshold(unsigned long longThreshold);
  int getButtonState();
  void setInverse(int inverse);
  void resetButtonState();
  virtual ~LMomentary();
  virtual void init(int buttonPin);
  virtual void poll();
  virtual void onSample();
  virtual void onPress();
  virtual void onLongPress();
  virtual void onRelease();
  virtual void onLongrelease();
};

#endif
