#include <Wire.h>
#include <RTClib.h>
#include <Adafruit_HTU21DF.h>
#include <Adafruit_MPL3115A2.h>

// I2C bus has RTC, Altimeter, and Humidity sensor
RTC_DS1307 RTC;
Adafruit_HTU21DF htu;
Adafruit_MPL3115A2 baro;
int htuValid;
int baroValid;

void setup () {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  htu = Adafruit_HTU21DF();
  if (!htu.begin()) {
    Serial.println("Couldn't find sensor!");
    htuValid = 0;
  } else {
    htuValid = 1;
  }
  baro = Adafruit_MPL3115A2();
  if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
    return;
    baroValid = 0;
  } else {
    baroValid = 1;
  }
}

void loop () {
  if (!RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
  } else {
    DateTime now = RTC.now();
    printDate(now);
  }
  if (htuValid) {
    printFTemp(htu.readTemperature());
    Serial.print(htu.readHumidity());
    Serial.println("% RH");
  }
  if (baroValid) {
    float pascals = baro.getPressure();
    // Our weather page presents pressure in Inches (Hg)
    Serial.print(pascals/3377); Serial.println(" Inches (Hg)");

    float altm = baro.getAltitude();
    Serial.print(altm*3.28084); Serial.println(" feet");

    printFTemp(baro.getTemperature());
  }
  delay(30000);
}

void printDate(DateTime dt) {
  Serial.print(dt.year(), DEC);
  Serial.print('-');
  Serial.print(dt.month(), DEC);
  Serial.print('-');
  Serial.print(dt.day(), DEC);
  Serial.print('T');
  Serial.print(dt.hour(), DEC);
  Serial.print(':');
  Serial.print(dt.minute(), DEC);
  Serial.print(':');
  Serial.print(dt.second(), DEC);
  Serial.println();
}

void printFTemp(float temp) {
  Serial.print(9*temp/5+32);
  Serial.println("*F");
}
