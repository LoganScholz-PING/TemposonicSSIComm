#include "Arduino.h"
#include "TemposonicSSICommunication.h"

#include "SPI.h" // TODO: Try to do an #IFDEF SPI somehow before including this

TempoSSI::TempoSSI(int DATA_PIN, int CLOCK_PIN, int BIT_COUNT)
{
    this->DATA_PIN = DATA_PIN;
    this->CLOCK_PIN = CLOCK_PIN;
    this->BIT_COUNT = BIT_COUNT;

    pinMode(this->DATA_PIN, INPUT);
    pinMode(this->CLOCK_PIN, OUTPUT);
    digitalWrite(this->CLOCK_PIN, HIGH);
}

TempoSSI_SPI::TempoSSI_SPI(int BIT_COUNT)
{
    this->BIT_COUNT = BIT_COUNT;
}

unsigned long TempoSSI::ReadPosition()
{
	// if in doubt, just use this implementation
    unsigned long data = 0x0;
    for (int i = 0; i < this->BIT_COUNT; i++)
    {
        digitalWrite(this->CLOCK_PIN, LOW);
        delayMicroseconds(5);
        digitalWrite(this->CLOCK_PIN, HIGH);
        delayMicroseconds(5);

        data |= digitalRead(this->DATA_PIN);
		data <<= 1;
    }
	return data;
}

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