#include "Arduino.h"
#include "TemposonicSSICommunication.h"

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
    unsigned long data = 0;
    unsigned long mask = 1;

    for (int i = 0; i < this->BIT_COUNT; i++)
    {
        data <<= 1;

        digitalWrite(this->CLOCK_PIN, LOW);
        delayMicroseconds(1);
        digitalWrite(this->CLOCK_PIN, HIGH);
        delayMicroseconds(1);

        data |= digitalRead(this->DATA_PIN);
    }

    for (int j = 0; j < this->BIT_COUNT; j++)
    {
        mask <<= 1;
        mask |= 1;
    }

    return (data & mask);
}