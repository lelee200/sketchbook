#include <LMomentary.h>
#include <Pollable.h>
#include <Poller.h>

class JS: public LMomentary {
  char *title;
public:
  JS(int buttonPin, char *title): LMomentary(buttonPin) {
    init(buttonPin, title);
  };
  
  void init (int buttonPin, char *title) {
    LMomentary::init(buttonPin);
    this->title = title;
    this->setInverse(1);
  }
  
  void printEvent(char *event) {
    Serial.print(event);
    Serial.print(" ");
    Serial.print(title);
    Serial.print("\n");
  }
  
  void onPress() {
    printEvent("Press");
  }
  
  void onLongPress() {
    printEvent("Long Press");
  }
  
  void onRelease() {
    printEvent("Release");
  }
  
  void onLongRelease() {
    printEvent("Long Release");
  }
  
//  void onSample() {
//    Serial.println(getButtonState());
//  }
};

Pollable *pb[5];
Poller poller(pb);

void setup() {
  Serial.begin(9600);
  pb[0] = new JS(2,"north");
  pb[1] = new JS(3,"east");
  pb[2] = new JS(4,"south");
  pb[3] = new JS(5,"west");
  pb[4] = 0;
}

void loop() {
  poller.loop();
  delay(10);
}


