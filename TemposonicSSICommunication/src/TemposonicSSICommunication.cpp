#include "Arduino.h"
#include "TemposonicSSICommunication.h"

//#include "SPI.h" // TODO: Try to do an #IFDEF SPI somehow before including this

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

/*
TempoSSI_SPI::TempoSSI_SPI(int BIT_COUNT)
{
    this->BIT_COUNT = BIT_COUNT;
}
*/

unsigned long TempoSSI::ReadPosition()
{
	int count = 0;
	bool done = false;
	unsigned long dataTempoRead1 = 0x0;
	unsigned long dataTempoRead2 = 0x0;
	unsigned long ret = 0x0;
	
	while(!done && count < 3)
	{
		// try 3 times to read the value twice
		// if the values don't match throw away the data
		// if 3 tries fail, return 0x0
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
			ret = 0xFFFF;
			done = false;
		}
		
		++count;
	}
	
	return ret;
}

/*
unsigned long TempoSSI_SPI::ReadPosition()
{
	// ESP32 Devkit V4 SPI pins:
	// MISO: 19 (VSPI_MISO)
	// SCK:  18 (VSPI_SCK)
	
	SPI.begin();
	SPI.setDataMode(SPI_MODE2);
	
	unsigned long data = 0x0;
	uint8_t out3 = SPI.transfer(0x0);
	uint8_t out2 = SPI.transfer(0x0);
	uint8_t out1 = SPI.transfer(0x0);
	delayMicroseconds(20); // indicate EOT
	data = (out3 << 16) | (out2 << 8) | out1;
	return data;
}
*/