/*
  
 The circuit:
 * LCD RS pin to digital pin 3
 * LCD Enable pin to digital pin 4
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 6
 * LCD D6 pin to digital pin 7
 * LCD D7 pin to digital pin 8
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 http://arduino.cc/en/Tutorial/LiquidCrystalDisplay
*/

// include the library code:
#include <LiquidCrystal.h>
#include <Pollable.h>
#include <AnalogInput.h>
#include <DHT11.h>
#include <Poller.h>
#include <EEPROM.h>

#define BYTES_VAL_T unsigned int

// #define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8
#define DATA_WIDTH   5

/* Width of pulse to trigger the shift register to read and latch.
*/
#define PULSE_WIDTH_USEC   5

#define HEATER 1
#define HUMID 2
#define LOW_FAN 4
#define HIGH_FAN 8
#define BACK_LIGHT 16

#define DEF_BKLT 45 

#define JOY_RIGHT 4
#define JOY_DOWN 8
#define JOY_LEFT 16
#define JOY_UP 2
#define JOY_CENTER 1

#define TO_HOLD 1000
#define TO_REPEAT 300

#define setOut(x) outputPins |= (x)
#define clearOut(x) outputPins &= ~(x)

#define SAVE_START 64
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

int ploadPin        = A5;  // Connects to Parallel load pin the 165
int clockEnablePin  = A4;  // Connects to Clock Enable pin the 165
int dataPin         = A3; // Connects to the Q7 pin the 165
int clockPin        = A2; // Connects to the Clock pin the 165

int outLatch =12;
int outClock = 11;
int outData = 13;

long Temps[2] = {0,0};
long Humids[2] = {0,0};
int outputPins = 0;
int inputPins = 0;
int min_temp = 80;
int min_humid = 40;
int updateFlag = 1;
unsigned long BacklightOffAt = 60000;

class TempHumid: public DHT11 {
  int num = 0;
public:
  TempHumid(): DHT11() {
    fahrenheit = 1;
  }
  void setNum(int num) {
    this->num = num;
  }
  void onSample() {
    Temps[num] = lastValue;
    Humids[num] = lastHumid;
    updateFlag = 1;
  }
};

Pollable *pa[4];
Poller *pl;

#define NUM_VALS 8
int Defaults[] = {80,2,40,2,2,2,0,0};
int Values[NUM_VALS];
int ResetDefaults = 0;

static char scratch[32];

class Entry {
public:
  char *name;
  int *ptrActual;
  int working;
  int minVal;
  int maxVal;
  char **valMap;
  
  Entry(char *name, int *ptrActual, int minVal, int maxVal, char **valMap = 0) {
    this->name = name;
    this->ptrActual = ptrActual;
    this->minVal = minVal;
    this->maxVal = maxVal;
    this->valMap = valMap;
  };
  
  void increment() {
    if (++working > maxVal)
      working = minVal;
  };
  
  void decrement() {
    if (--working < minVal)
      working = maxVal;
  };
  
  void initVal() {
    working = *ptrActual;
  };
  
  int commit() {
    if (working == *ptrActual)
      return 0;
    *ptrActual = working;
    return 1;
  };
  
  char *ValueToString() {
    if (valMap) {
      return valMap[working];
    } else {
      int quotient = working;
      char *ptr = scratch+31;
      *(ptr--) = '\0';
      do {
        *(ptr--) = '0' + (quotient % 10);
        quotient /= 10;
      } while (quotient);
      return ++ptr;
    }
  };
};

char *FanMap[] = {"Auto","On"};
char *NoYes[] = {"No","Yes"};

#define OFFSET_T_LOW 0
#define OFFSET_T_RNG 1
#define OFFSET_H_LOW 2
#define OFFSET_H_RNG 3
#define OFFSET_T_DIF 4
#define OFFSET_H_DIF 5
#define OFFSET_F_U 6
#define OFFSET_F_L 7

Entry Menu[] = {
  Entry("Low Temp", Values, 60, 110),
  Entry("Temp Rng", Values+1, 0, 20),
  Entry("Low Humid", Values+2, 20, 80),
  Entry("Humid Rng", Values+3, 0, 20),
  Entry("Temp Dif", Values+4, 0, 20),
  Entry("Humid Dif", Values+5, 0, 20),
  Entry("Top Fan", Values+6, 0, 1, FanMap),
  Entry("Base Fan", Values+7, 0, 1, FanMap),
  Entry("Set Def", &ResetDefaults, 0, 1, NoYes)
};

int EntryCount = sizeof(Menu)/sizeof(Entry);
int MenuIndex = 0;

void PrepareMenu () {
  for (int index = 0; index < EntryCount; index++) {
    Menu[index].initVal();
    MenuIndex = 0;
  }
  updateFlag = 1;
}

void MenuNext() {
  if (++MenuIndex >= EntryCount) {
    MenuIndex = 0;
  }
  updateFlag = 1;
}

void MenuPrev() {
  if (--MenuIndex < 0) {
    MenuIndex = EntryCount - 1;
  }
  updateFlag = 1;
}

void MenuIncr() {
  Menu[MenuIndex].increment();
  updateFlag = 1;
}

void MenuDecr() {
  Menu[MenuIndex].decrement();
  updateFlag = 1;
}

void CommitSettings() { 
  for (int index = 0; index < EntryCount; index++) {
    Menu[index].commit();
  }
}

void LoadFromFlash() {
  for (int index = 0; index < NUM_VALS; index++) {
    Values[index] = EEPROM.read(SAVE_START+index);
  }
}

void SaveToFlash() {
  for (int index = 0; index < NUM_VALS; index++) {
    EEPROM.write(SAVE_START+index,Values[index]);
  }
}

void DoResetDefaults() {
  for (int index = 0; index < sizeof (Defaults)/sizeof (int); index++) {
    Values[index] = Defaults[index];
  }
  ResetDefaults = 0;
}

char *GetEntryName() {
  return Menu[MenuIndex].name;
}

char *GetEntryValue() {
  return Menu[MenuIndex].ValueToString();
}

void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  TempHumid *th = new TempHumid();
  th->init(9,1300);
  pa[0] = th;
  th = new TempHumid();
  th->init(10,1300);
  th->setNum(1);
  pa[1] = th;
  pa[2] = 0;
  pl = new Poller(pa);
  pinMode(ploadPin, OUTPUT);
  pinMode(clockEnablePin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, INPUT);
  //set pins to output so you can control the shift register
  pinMode(outLatch, OUTPUT);
  pinMode(outClock, OUTPUT);
  pinMode(outData, OUTPUT);
//  Serial.begin(9600);
  LoadFromFlash();
}

int State = 0;
long timeoutat = 0;
int Count = 0;

void showDisplay() {
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  for (int num = 0; num < 2; num++) {
    if (Temps[num] > 0) {
      lcd.print(Temps[num]);
    } else {
      lcd.print("---");
    }
    lcd.print("/");
    if (Humids[num] > 0) {
      lcd.print(Humids[num]);
    } else {
      lcd.print("---");
    }
    lcd.print(" ");
  }
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  switch (State) {
    case 0:
    case 1:
    case 10:
    {
      int mask = 128;
      for (int count = 0; count < 8; count++, mask >>= 1) {
        lcd.print((mask & outputPins) ? '+': '-');
      }
    }
    break;
    
    case 3:
    case 4:
    case 5:
    case 6:
    lcd.print(GetEntryName());
    lcd.print(": ");
    lcd.print(">>");
    lcd.print(GetEntryValue());
    break;
    
    default:
    lcd.print(">>");
    lcd.print(GetEntryName());
    lcd.print(": ");
    lcd.print(GetEntryValue());
    break;
  }
}

void setBacklightOn(unsigned long t) {
  BacklightOffAt = millis() + (t) * 1000;
}

/* This function is essentially a "shift-in" routine reading the
 * serial Data from the shift register chips and representing
 * the state of those pins in an unsigned integer (or long).
*/
BYTES_VAL_T read_shift_regs()
{
    long bitVal;
    BYTES_VAL_T bytesVal = 0;

    /* Trigger a parallel Load to latch the state of the data lines,
    */
    digitalWrite(clockEnablePin, HIGH);
    digitalWrite(ploadPin, LOW);
    delayMicroseconds(PULSE_WIDTH_USEC);
    digitalWrite(ploadPin, HIGH);
    digitalWrite(clockEnablePin, LOW);

    /* Loop to read each bit value from the serial out line
     * of the SN74HC165N.
    */
    for(int i = 0; i < DATA_WIDTH; i++)
    {
        bitVal = digitalRead(dataPin);

        /* Set the corresponding bit in bytesVal.
        */
        bytesVal |= (bitVal << ((DATA_WIDTH-1) - i));

        /* Pulse the Clock (rising edge shifts the next bit).
        */
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(PULSE_WIDTH_USEC);
        digitalWrite(clockPin, LOW);
    }
    return(bytesVal);
}

void Edge(int input) {
  long current = millis();
  switch (State) {
    case 0:
    if (JOY_CENTER == input) {
      timeoutat = current + TO_HOLD;
      State = 1;
    }
    break;
    
    case 1:
    if (!input) {
      timeoutat = 0;
      State = 0;
    } else if (JOY_CENTER == input) {
      if (current >= timeoutat) {
        timeoutat = 0;
        State = 11;
        updateFlag = 1;
      }
    }
    break;
    
    case 2:
    switch (input) {
      case 0:
      timeoutat = 0;
      break;
      
      case JOY_UP:
      if (current >= timeoutat) {
        MenuPrev();
        timeoutat = current + TO_HOLD;
      }
      break;
      
      case JOY_DOWN:
      if (current >= timeoutat) {
        MenuNext();
        timeoutat = current + TO_HOLD;
      }
      break;
      
      case JOY_RIGHT:
      timeoutat = 0;
      State = 3;
      updateFlag = 1;
      break;
      
      case JOY_LEFT:
      timeoutat = 0;
      State = 12;
      MenuPrev();
      break;
      
      case JOY_CENTER:
      timeoutat = current + TO_HOLD;
      State = 9;
    }
    break;
    
    case 3:
    switch (input) {
      case JOY_UP:
      MenuIncr();
      timeoutat = current + TO_HOLD;
      State = 4;
      break;
      
      case JOY_DOWN:
      MenuDecr();
      timeoutat = current + TO_HOLD;
      State = 5;
      break;
      
      case JOY_LEFT:
      timeoutat = 0;
      State = 13;
      updateFlag = 1;
      break;
      
      case JOY_CENTER:
      timeoutat = current + TO_HOLD;
      State = 6;
      break;
    }
    break;
    
    case 4:
    if (JOY_UP == input) {
      if (current >= timeoutat) {
        MenuIncr();
        timeoutat = current + TO_REPEAT;
      }
    } else {
      timeoutat = 0;
      State = 3;
    }
    break;
    
    case 5:
    if (JOY_DOWN == input) {
      if (current >= timeoutat) {
        MenuDecr();
        timeoutat = current + TO_REPEAT;
      }
    } else {
      timeoutat = 0;
      State = 3;
    }
    break;
    
    case 6:
    if (JOY_CENTER == input) {
      if (current > timeoutat) {
        CommitSettings();
        if (ResetDefaults) {
          DoResetDefaults();
        }
        SaveToFlash();
        State = 10;
        timeoutat = 0;
        MenuIndex = 0;
        updateFlag = 1;
      }
    } else {
      State = 3;
      timeoutat = 0;
    }
    break;
    
    case 7:
    if (JOY_UP != input) {
      State = 2;
    }
    break;

    case 8:
    if (JOY_DOWN != input) {
      State = 2;
    }
    break;
    
    case 9:
    if (JOY_CENTER == input) {
      if (current > timeoutat) {
        CommitSettings();
        if (ResetDefaults) {
          DoResetDefaults();
        }
        SaveToFlash();
        State = 10;
        timeoutat = 0;
        MenuIndex = 0;
        updateFlag = 1;
      }
    } else {
      State = 2;
      timeoutat = 0;
    }
    break;
    
    case 10:
    if (JOY_CENTER != input) {
      State = 0; 
    }
    break;

    case 11:
    if (!input) {
      State = 2;
      PrepareMenu();
      timeoutat = 0;
      updateFlag = 1;
    }
    break;
    
    case 12:
    if (!input) {
      State = 0;
      timeoutat = 0;
      Count = 0;
      updateFlag = 1;
    }
    break;
    
    case 13:
    if (!input) {
      State = 2;
      timeoutat = 0;
      Count = 0;
    }
    break;
  }
}

int getJoystickInput() {
  int values = read_shift_regs() & 31;
  if (values) {
    setBacklightOn(60);
    if (values & (JOY_DOWN|JOY_UP)) {
      return values & (JOY_DOWN|JOY_UP);
    }
    if (values & (JOY_LEFT|JOY_RIGHT)) {
      return values & (JOY_LEFT|JOY_RIGHT);
    }
    if (values & JOY_CENTER) {
      return values & JOY_CENTER;
    }
  }
}

void loop() {
  pl->loop();
  int direct = getJoystickInput();
  Edge(direct);
  int oldPinVals = outputPins & 15;
  if (Temps[0] > Values[OFFSET_T_LOW] + Values[OFFSET_T_RNG] || Temps[1] > Values[OFFSET_T_LOW] + Values[OFFSET_T_RNG]) {
    clearOut(HEATER);
    if (!Values[OFFSET_F_L]) clearOut(LOW_FAN);
  } else if (Temps[0] < Values[OFFSET_T_LOW] || Temps[1] < Values[OFFSET_T_LOW]) {
    setOut(HEATER);
  }
  if (Humids[0] > Values[OFFSET_H_LOW] + Values[OFFSET_H_RNG] || Humids[1] > Values[OFFSET_H_LOW] + Values[OFFSET_H_RNG]) {
    clearOut(HEATER);
    if (!Values[OFFSET_F_L]) clearOut(LOW_FAN);
  } else if (Humids[0] < Values[OFFSET_T_LOW] || Humids[1] < Values[OFFSET_T_LOW]) {
    setOut(HUMID);
  }
  if (Values[OFFSET_F_L]) {
    setOut(LOW_FAN);
  } else if (outputPins & (HEATER|HUMID)) {
    setOut(LOW_FAN);
  } else 
    clearOut(LOW_FAN);
  
  if (Values[OFFSET_F_U])
    setOut(HIGH_FAN);
  else if (Temps[0] > Temps[1] + Values[OFFSET_T_DIF] || Temps[1] > Temps[0] + Values[OFFSET_T_DIF]
           || Humids[0] > Humids[1] + Values[OFFSET_H_DIF] || Humids[1] > Humids[0] + Values[OFFSET_H_DIF]) {
    setOut(HIGH_FAN);
  } else {
    clearOut(HIGH_FAN);
  }
  
  if (oldPinVals != (outputPins & 15)) setBacklightOn(DEF_BKLT);
  unsigned long now = millis();
  if (BacklightOffAt < now) {
    clearOut(BACK_LIGHT);
  } else {
    setOut(BACK_LIGHT);
  }
  
  digitalWrite(outLatch, LOW);
  // shift out the bits:
  shiftOut(outData, outClock, MSBFIRST, outputPins);  
  //take the latch pin high so the LEDs will light up:
  digitalWrite(outLatch, HIGH);
  if (updateFlag) showDisplay();
  updateFlag = 0;
  delay(10);
}

