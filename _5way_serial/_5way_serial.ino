/*
 * SN74HC165N_shift_reg
 *
 * Program to shift in the bit values from a SN74HC165N 8-bit
 * parallel-in/serial-out shift register.
 *
 * This sketch demonstrates reading in 16 digital states from a
 * pair of daisy-chained SN74HC165N shift registers while using
 * only 4 digital pins on the Arduino.
 *
 * You can daisy-chain these chips by connecting the serial-out
 * (Q7 pin) on one shift register to the serial-in (Ds pin) of
 * the other.
 * 
 * Of course you can daisy chain as many as you like while still
 * using only 4 Arduino pins (though you would have to process
 * them 4 at a time into separate unsigned long variables).
 * 

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
*/ 

// How many shift register chips are daisy-chained.
#include <LiquidCrystal.h>

#define NUMBER_OF_SHIFT_CHIPS   1

/* Width of data (how many ext lines).
*/
// #define DATA_WIDTH   NUMBER_OF_SHIFT_CHIPS * 8
#define DATA_WIDTH   5

/* Width of pulse to trigger the shift register to read and latch.
*/
#define PULSE_WIDTH_USEC   5

/* Optional delay between shift register reads.
*/
#define POLL_DELAY_MSEC   1

/* You will need to change the "int" to "long" If the
 * NUMBER_OF_SHIFT_CHIPS is higher than 2.
*/
#define BYTES_VAL_T unsigned int

int ploadPin        = A5;  // Connects to Parallel load pin the 165
int clockEnablePin  = A4;  // Connects to Clock Enable pin the 165
int dataPin         = A3; // Connects to the Q7 pin the 165
int clockPin        = A2; // Connects to the Clock pin the 165

int outLatch =12;
int outClock = 11;
int outData = 13;

BYTES_VAL_T pinValues;
BYTES_VAL_T oldPinValues;

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

/* Dump the list of zones along with their current status.
*/

char *Map = "RDLUC";
LiquidCrystal lcd(3, 4, 5, 6, 7, 8);
#define JOY_RIGHT 4
#define JOY_DOWN 8
#define JOY_LEFT 16
#define JOY_UP 2
#define JOY_CENTER 1
int cursorx = 0;
int cursory = 0;

void display_pin_values()
{
    lcd.setCursor(cursorx,cursory);
    lcd.print(" ");
    if (pinValues & JOY_RIGHT) {
      if (++cursorx > 15)
          cursorx = 15;
    } else if (pinValues & JOY_LEFT) {
      if (--cursorx < 0)
          cursorx = 0;
    }
    if (pinValues & JOY_DOWN) {
      if (++cursory > 1)
          cursory = 1;
    } else if (pinValues & JOY_UP) {
      if (--cursory < 0)
          cursory = 0;
    }
    lcd.setCursor(cursorx,cursory);
    if (pinValues & JOY_CENTER) {
      lcd.print("+");
    } else {
      lcd.print("*");
    }
}

void setup()
{
    /* Initialize our digital pins...
    */
    pinMode(ploadPin, OUTPUT);
    pinMode(clockEnablePin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT);
    pinMode(outLatch, OUTPUT);
    pinMode(outClock, OUTPUT);
    pinMode(outData, OUTPUT);

    digitalWrite(clockPin, LOW);
    digitalWrite(ploadPin, HIGH);

    /* Read in and display the pin states at startup.
    */
    lcd.begin(16, 2);
    lcd.print("hello");
    pinValues = read_shift_regs();
    display_pin_values();
    oldPinValues = pinValues;
}

void loop()
{
    /* Read the state of all zones.
    */
    pinValues = read_shift_regs();

    /* If there was a chage in state, display which ones changed.
    */
    if(pinValues != oldPinValues)
    {
        display_pin_values();
        oldPinValues = pinValues;
    }

    digitalWrite(outLatch, LOW);
    // shift out the bits:
    shiftOut(outData, outClock, MSBFIRST, 1 << (((cursorx + cursory) & 7)));  

    //take the latch pin high so the LEDs will light up:
    digitalWrite(outLatch, HIGH);
    delay(POLL_DELAY_MSEC);
}


