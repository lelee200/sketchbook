#include <LiquidCrystal.h>

/* Accelerometer display to LCD

/* LCD
The circuit:
 * LCD RS pin to digital pin 7
 * LCD Enable pin to digital pin 6
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

void setup () {
  lcd.begin(16, 2);
}

void loop () {
  int x = analogRead(A0);
  delay(5);
  x += analogRead(A0);
  lcd.setCursor(0,0);
  lcd.print (x/2);
  lcd.print ("     ");
}

