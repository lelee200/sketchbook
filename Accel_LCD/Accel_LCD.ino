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
  lcd.print("   X     Y    Z");
}

void intToString (char *buff, size_t len, int value) {
  char *ptr = buff;
  for (int count = 0; count < len; count++, ptr++) {
    *ptr = ' ';
  }
  *(ptr--) = '\0';
  if (!value) {
    *ptr = '0';
    return;
  }
  while (value) {
    int remainder = value % 10;
    value /= 10;
    *(ptr--) = '0' + remainder;
    if (buff >= ptr) {
      return;
    }
  }
}

void loop () {
  char buff[6];
  delay(5);
  int x = analogRead(A0);
  delay(5);
  x += analogRead(A0);
  delay(5);
  int y = analogRead(A1);
  delay(5);
  y += analogRead(A1);
  delay(5);
  int z = analogRead(A2);
  delay(5);
  z += analogRead(A2);
  lcd.setCursor(0,1);
  intToString (buff, sizeof(buff), x/2);
  lcd.print(buff);
  intToString (buff, sizeof(buff), y/2);
  lcd.print(buff);
  intToString (buff, sizeof(buff), z/2);
  lcd.print(buff);
  delay(10);
}

