/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:

class PrimeGen {
  int start = 3;
  public:
    void seed (int newVal) { start = newVal; }
    int next() {
      int candidate = start;
      do {
        int quotient = candidate;
        int modulo;
        int divisor;
        for (divisor = 2; divisor < quotient && divisor < start; divisor++) {
			modulo = candidate % divisor;
			if (!modulo) break;
			quotient = candidate / divisor;
        }
        if (modulo) {
          start = candidate > 30 ? 3 : candidate + 1;
          return candidate;
        } else {
          candidate++;
        }
      } while (1);
      return 1;
    }
};

class Light {
  int iter = 1;
  int prev = 0;
  int curr = 1;
  int pin = -1;
  int limit = 10007;
  PrimeGen *pg;
  public:
  Light() {};
  Light (int pin, PrimeGen *pg) {
    init (pin, pg);
  }
  
  Light* init (int pin, PrimeGen *pg) {
    this->pin = pin;
    this->pg = pg;
    limit = pg->next();
    curr = pg->next();
    prev = pg->next();
    return this;
  }
  
  void setup () {
    // initialize the digital pin as an output.
    pinMode(pin, OUTPUT);
  }
  
  void on() {
    digitalWrite(pin, HIGH);
  }
  
  void off() {
    digitalWrite(pin, LOW);
  }
  
  void toggle() {
    digitalWrite(pin, LOW == digitalRead(pin) ? HIGH : LOW);
  }

  void loop() {
    if (0 >= iter--) {
      toggle();
      
      iter = curr + prev;
      if (!iter && !curr) ++iter;
      if (iter > limit) {
        limit = pg->next();
        iter %= limit;
      }
      prev = curr;
      curr = iter;
    }
  }
}; 

#define NUM_LEDS 12
const int LEDs[] = {13,12,11,10,9,8,7,6,5,4,3,2};
Light Lights[NUM_LEDS];
PrimeGen PG;
// the setup routine runs once when you press reset:
void setup() {
  PG.seed(9);
  for (int index = 0; index < NUM_LEDS; index++)
    Lights[index].init(LEDs[index],&PG)->setup();
}

// the loop routine runs over and over again forever:
void loop() {
  for (int index = 0; index < NUM_LEDS; index++) {
    Lights[index].loop();
    delay(5);               // wait for 1/2 second
  }
} 
