/*
 * dht11.cpp
 *
 *  Created on: Dec 22, 2013
 *      Author: lance
 */

#include <DHT11.h>
#include <Arduino.h>

#define TIMEOUT 10000


DHT11::DHT11() : Pollable() {}

DHT11::~DHT11() {}

void DHT11::setFahrenheit(long fahrenheit) {
	this->fahrenheit = fahrenheit;
}

// Adapted from dht.cpp 0.1.0 by Rob Tillaart (01/04/2011)

void DHT11::poll() {
	uint8_t bits[5];  // buffer to receive data
	// INIT BUFFERVAR TO RECEIVE DATA
	uint8_t cnt = 7;
	uint8_t idx = 0;

	// EMPTY BUFFER
	for (int i=0; i< 5; i++) bits[i] = 0;
	lastValue = -1000;
	lastHumid = -1;

	// REQUEST SAMPLE
	pinMode(inputPin, OUTPUT);
	digitalWrite(inputPin, LOW);
	delay(20);
	digitalWrite(inputPin, HIGH);
	delayMicroseconds(40);
	pinMode(inputPin, INPUT);

	// GET ACKNOWLEDGE or TIMEOUT
	unsigned int loopCnt = TIMEOUT;
	while(digitalRead(inputPin) == LOW)
		if (loopCnt-- == 0) {
			return;
		}

	loopCnt = TIMEOUT;
	while(digitalRead(inputPin) == HIGH)
		if (loopCnt-- == 0) {
			return;
		}

	// READ THE OUTPUT - 40 BITS => 5 BYTES
	for (int i=0; i<40; i++)
	{
		loopCnt = TIMEOUT;
		while(digitalRead(inputPin) == LOW)
			if (loopCnt-- == 0) {
				return;
			}

		unsigned long t = micros();

		loopCnt = TIMEOUT;
		while(digitalRead(inputPin) == HIGH)
			if (loopCnt-- == 0) {
				return;
			}

		if ((micros() - t) > 40) bits[idx] |= (1 << cnt);
		if (cnt == 0)   // next byte?
		{
			cnt = 7;
			idx++;
		}
		else cnt--;
	}

	if (bits[4] != bits[0] + bits[2]) {
		return;
	}

	lastHumid = bits[0];
	lastValue = bits[2];
	if (fahrenheit) {
		lastValue = 9 * lastValue / 5 + 32;
	}
	onSample();
}
