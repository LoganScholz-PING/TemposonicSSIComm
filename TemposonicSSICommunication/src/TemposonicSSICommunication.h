#ifndef TEMPOSONICSSICOMMUNICATION_H
#define TEMPOSONICSSICOMMUNICATION_H

class TempoSSI
{
    public: 
        TempoSSI(int DATA_PIN, int CLOCK_PIN, int BIT_COUNT);

        unsigned long ReadPosition();
    
    private:
        int DATA_PIN;
        int CLOCK_PIN;
        int BIT_COUNT;
};

#endif