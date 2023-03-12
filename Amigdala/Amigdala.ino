/*
Amigdala Box 
 Turns on and off a light emitting diode(LED) connected to digital  
 pin 13, when pressing a pushbutton attached to pin 2. 
 
 
 The circuit:
 * LED attached from pin 13 to ground 
 * pushbutton attached to pin 2 from +5V
 * 10K resistor attached to pin 2 from ground
 
 * Note: on most Arduinos there is already an LED on the board
 attached to pin 13.
 
 
 created 2005
 by DojoDave <http://www.0j0.org>
 modified 30 Aug 2011
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/Button
 */

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int HornPin =  12;      // the number of the LED pin
int State = 0;
unsigned long Timeout = 0;

void setup() {
  // initialize the LED pin as an output:
  digitalWrite(HornPin, LOW);
  pinMode(HornPin, OUTPUT);     
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
//  Serial.begin(9600);
}

void PrintState() {
  Serial.print(State);
  Serial.print("|");
  Serial.println(Timeout);
}

void SetTimeout(unsigned long variance, unsigned long minimum) {
  unsigned long ahora = millis();
  Timeout = ahora + 1000 * (ahora % variance + minimum);
}

void loop(){
  // read the state of the pushbutton value:
  
  switch (State) {
    case 0:
      if (digitalRead(buttonPin)) {
        State = 1;
//        PrintState();
      }
      break;
    case 1:
      if (!digitalRead(buttonPin)) {
        State = 2;
        SetTimeout(7,3);
//        PrintState();
      }
      break;
    case 2:
      if (millis() > Timeout) {
        State = 3;
        SetTimeout(3,1);
        digitalWrite(HornPin, HIGH);
//        PrintState();
      }
    break;
    case 3:
      if (millis() > Timeout) {
        State = 4;
        Timeout = millis() + 3000;
        digitalWrite(HornPin, LOW);
//        PrintState();
      }
    break;
    case 4:
      if (millis() > Timeout) {
        State = 0;
        Timeout = 0;
        digitalWrite(HornPin, LOW);
//        PrintState();
      }
  }
  delay(20);
}
