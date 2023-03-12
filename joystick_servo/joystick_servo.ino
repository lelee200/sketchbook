#include <Servo.h> 

int SensorPin = A0;    // select the input pin for the potentiometer
int ServoPin = 3;      // select the pin for the LED
Servo myservo;

void setup() {
  myservo.attach(ServoPin);  // attaches the servo on pin 9 to the servo object  
}

void loop() {
  // read the value from the sensor:
  int sensorValue = (analogRead(SensorPin));
  myservo.write(map(sensorValue, 0, 1023, 0, 179));     // scale it to use it with the servo (value between 0 and 180)
  delay(15);
}
