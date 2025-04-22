#ifndef TEMPOSONICSSICOMMUNICATION_H
#define TEMPOSONICSSICOMMUNICATION_H

/*

There are two wiring strategies:
1. Ground CLOCK- and DATA-. Then wire CLOCK+ and DATA+ into the Arduino Mega's digital input pins
2. Use a Differential to single-ended converter (like the MAX485) to convert the differential signals to single-ended signals

Library written specifically for the Temposonics LVDT part # EL00300UD841S2B5100

*/

class TempoSSI
{
	// Bit-Bang read methodology
    public: 
        TempoSSI(int DATA_PIN, int CLOCK_PIN, int BIT_COUNT);

        unsigned long ReadPosition();
    
    private:
        int DATA_PIN;
        int CLOCK_PIN;
        int BIT_COUNT;
};
/*
class TempoSSI_SPI
{
	// SPI read methodology
    public: 
        TempoSSI_SPI(int BIT_COUNT);

        unsigned long ReadPosition();
    
    private:
        int BIT_COUNT;
};
*/
#endif