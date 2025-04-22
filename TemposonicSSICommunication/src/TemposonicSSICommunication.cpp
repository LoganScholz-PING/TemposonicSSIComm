#include "Arduino.h"
#include "TemposonicSSICommunication.h"

bool debugTemposonicSSICommunication = false;

TempoSSI::TempoSSI(int DATA_PIN, int CLOCK_PIN, int BIT_COUNT)
{
    this->DATA_PIN = DATA_PIN;
    this->CLOCK_PIN = CLOCK_PIN;
    this->BIT_COUNT = BIT_COUNT;

    pinMode(this->DATA_PIN, INPUT);
    pinMode(this->CLOCK_PIN, OUTPUT);
    digitalWrite(this->CLOCK_PIN, HIGH);
}

unsigned long TempoSSI::ReadPosition()
{
	int count = 0;
	bool done = false;
	unsigned long dataTempoRead1 = 0x0;
	unsigned long dataTempoRead2 = 0x0;
	unsigned long ret = 0xFFFF;
	
	while(!done && count < 3)
	{
		// try multiple times to read the same value twice
		// if the values don't match throw away the data
		// if multiple tries fail, return 0xFFFF to indicate failure
		for (int p = 0; p < this->BIT_COUNT; ++p)
		{
			digitalWrite(this->CLOCK_PIN, LOW);
			delayMicroseconds(10);
			digitalWrite(this->CLOCK_PIN, HIGH);
			delayMicroseconds(10);

			dataTempoRead1 |= digitalRead(this->DATA_PIN);
			dataTempoRead1 <<= 1;
		}
		
		// let LVDT output register update
		delayMicroseconds(20);
		
		for (int o = 0; o < this->BIT_COUNT; ++o)
		{
			digitalWrite(this->CLOCK_PIN, LOW);
			delayMicroseconds(10);
			digitalWrite(this->CLOCK_PIN, HIGH);
			delayMicroseconds(10);

			dataTempoRead2 |= digitalRead(this->DATA_PIN);
			dataTempoRead2 <<= 1;
		}
		
		if(debugTemposonicSSICommunication)
		{
			Serial.print(">>> Count ");
			Serial.print(count);
			Serial.print(" --> dataTempoRead1[");
			Serial.print(dataTempoRead1);
			Serial.print("] // dataTempoRead2[");
			Serial.print(dataTempoRead2);
			Serial.println("]");
		}
		
		if(dataTempoRead1 == dataTempoRead2)
		{
			done = true;
			ret = dataTempoRead1;
		}
		else
		{
			dataTempoRead1 = 0x0;
			dataTempoRead2 = 0x0;
			done = false;
		}
		
		++count;
	}
	
	return ret;
}

// much faster but will sometimes give spurious values that need to be averaged out
unsigned long TempoSSI::ReadPositionRaw()
{
	unsigned long dataTempoRead = 0x0;
	
	for (int o = 0; o < this->BIT_COUNT; ++o)
	{
		digitalWrite(this->CLOCK_PIN, LOW);
		delayMicroseconds(10);
		digitalWrite(this->CLOCK_PIN, HIGH);
		delayMicroseconds(10);

		dataTempoRead |= digitalRead(this->DATA_PIN);
		dataTempoRead <<= 1;
	}
	
	return dataTempoRead;
}