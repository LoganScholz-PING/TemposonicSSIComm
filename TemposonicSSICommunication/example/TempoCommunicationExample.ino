#include <Arduino.h>
#include <TemposonicSSICommunication.h>

// *************************************************************************
// **** Arduino Mega Pins ************************************************** 
//#define DATA_PIN  52   // Driver Input (Data from LVDT to Arduino)
//#define CLOCK_PIN 50   // Receiver output (Clock from Arduino to LVDT)
// **** ESP32 DevkitC V4 Pins **********************************************
#define DATA_PIN  19     // Driver Input (Data from LVDT to Arduino)
#define CLOCK_PIN 18     // Receiver output (Clock from Arduino to LVDT)
// *************************************************************************

#define BIT_COUNT 24

TempoSSI Tempo = TempoSSI(DATA_PIN, CLOCK_PIN, BIT_COUNT);

void setup() 
{
  Serial.begin(115200);

  while(!Serial) { /* wait */ }

  Serial.println("Entering main loop...\n\n");
}

unsigned long readingBitBang = 0;
unsigned long readingBitBangRaw = 0;
void loop() 
{
  readingBitBang = Tempo.ReadPosition();
  delayMicroseconds(20);
  readingBitBangRaw = Tempo.ReadPositionRaw();

  if(readingBitBang != 0xFFFF)
  {
    Serial.print("SMOOTH READING:    ");
    Serial.print(readingBitBang);
    Serial.print(" (");
    Serial.print(readingBitBang, BIN);
    Serial.println(")");
  }
  else
  {
    Serial.println("SMOOTH READING:    *ERROR* (*ERROR*)");
  }

  Serial.print("RAW READING:       ");
  Serial.print(readingBitBangRaw);
  Serial.print(" (");
  Serial.print(readingBitBangRaw, BIN);
  Serial.println(")");

  Serial.println("**********************");

  delay(250);
}
