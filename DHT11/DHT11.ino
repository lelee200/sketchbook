#include <dht.h>

dht *dht_reader = 0;
#define HEATER 3
void setup()
{
  dht_reader = new dht();
  Serial.begin(9600);
  Serial.println();
  pinMode(HEATER,OUTPUT);
  digitalWrite(HEATER,LOW);
  delay(2000);
}

void loop()
{
  double fTemp;
  dht_reader->read11(2);
  fTemp = 9.0*dht_reader->temperature/5.0+32.0;
  Serial.print("T=");
  Serial.print(fTemp);
  Serial.print(" H=");
  Serial.print(dht_reader->humidity);
  Serial.println("%");
  delay(1000);
  if (fTemp > 80.0) {
    digitalWrite(HEATER,LOW);
  } else if (fTemp < 75.0) {
    digitalWrite(HEATER,HIGH);
  }
}


