// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
 
#include <Wire.h>
#include "RTClib.h"
 
RTC_DS1307 RTC;
RTC_Millis RTCM;
 
void setup () {
    Serial.begin(115200);
    Wire.begin();
    RTC.begin();
    RTC.writeSqwPinMode(ON);
  //following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
    RTCM.begin(RTC.now());
  
}
 
void loop () {
    delay(2000);
    if (!RTC.isrunning()) {
      Serial.println("RTC is NOT running!");
    } else {
      DateTime now = RTC.now();
      DateTime now2 = RTCM.now();
      long ahora = now.unixtime();
      long ahora2 = now2.unixtime();
      long secondsOn = millis() / 1000L;

//      Serial.print("1:");
//      Serial.print(now.year(), DEC);
//      Serial.print('/');
//      Serial.print(now.month(), DEC);
//      Serial.print('/');
//      Serial.print(now.day(), DEC);
//      Serial.print(' ');
//      Serial.print(now.hour(), DEC);
//      Serial.print(':');
//      Serial.print(now.minute(), DEC);
//      Serial.print(':');
//      Serial.print(now.second(), DEC);
//      Serial.println();
      Serial.print("since 1970 = ");
      Serial.print(ahora);
      Serial.println("s");
 
//      Serial.print("2:");
//      Serial.print(now2.year(), DEC);
//      Serial.print('/');
//      Serial.print(now2.month(), DEC);
//      Serial.print('/');
//      Serial.print(now2.day(), DEC);
//      Serial.print(' ');
//      Serial.print(now2.hour(), DEC);
//      Serial.print(':');
//      Serial.print(now2.minute(), DEC);
//      Serial.print(':');
//      Serial.print(now2.second(), DEC);
//      Serial.println();
    
      Serial.print("since 1970 = ");
      Serial.print(ahora2);
      Serial.println("s");
    
      long diff = ahora - ahora2;
      Serial.print("diff = ");
      Serial.print(diff);
      Serial.print("s seconds on = ");
      Serial.print(secondsOn);
      Serial.print("s drift = ");
      Serial.print(60.0 * diff / secondsOn);
      Serial.println("s/m");
 
    // calculate a date which is 7 days and 30 seconds into the future
    //DateTime future (now.unixtime() + 7 * 86400L + 30);
 
//      Serial.print(" now + 7d + 30s: ");
//      Serial.print(now.year(), DEC);
//      Serial.print('/');
//      Serial.print(now.month(), DEC);
//      Serial.print('/');
//      Serial.print(now.day(), DEC);
//      Serial.print(' ');
//      Serial.print(now.hour(), DEC);
//      Serial.print(':');
//      Serial.print(now.minute(), DEC);
//      Serial.print(':');
//      Serial.print(now.second(), DEC);
//      Serial.println();
 
//      Serial.println();
    }
    delay(10000);
}
