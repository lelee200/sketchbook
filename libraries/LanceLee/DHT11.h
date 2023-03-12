#ifndef DHT11_H
#define DHT11_H
#include <Pollable.h>

class DHT11: public Pollable {
public:
  long lastHumid;
  long fahrenheit = 0;

  DHT11();
  ~DHT11();

  virtual void poll();
  virtual void onSample() = 0;
  void setFahrenheit(long fahrenheit = 1);
};
#endif
