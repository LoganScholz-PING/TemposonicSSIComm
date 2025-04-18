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
    unsigned long data = 0;
    unsigned long mask = 1;

    for (int i = 0; i < this->BIT_COUNT; i++)
    {
        data <<= 1;

        digitalWrite(this->CLOCK_PIN, LOW);
        delayMicroseconds(10);
        digitalWrite(this->CLOCK_PIN, HIGH);
        delayMicroseconds(10);

        data |= digitalRead(this->DATA_PIN);
    }

    for (int j = 0; j < this->BIT_COUNT; j++)
    {
        mask <<= 1;
        mask |= 1;
    }

    return (data & mask);
}

unsigned long TempoSSI_SPI::ReadPosition()
{
	// ESP32 Devkit V4 SPI pins:
	// MISO: 19 (VSPI_MISO)
	// SCK:  18 (VSPI_SCK)
	
	SPI.begin();
	SPI.setDataMode(SPI_MODE2);
	
	uint8_t out4 = SPI.transfer(0x0);
	uint8_t out3 = SPI.transfer(0x0);
	uint8_t out2 = SPI.transfer(0x0);
	uint8_t out1 = SPI.transfer(0x0);
	delayMicroseconds(20); // indicate EOT
	unsigned long output = (out4 << 24) | (out3 << 16) | (out2 << 8) | out1;
	
	unsigned long mask = 0;
	for (int j = 0; j < this->BIT_COUNT; j++)
    {
        mask <<= 1;
        mask |= 1;
    }
	
	return (output & mask);
}