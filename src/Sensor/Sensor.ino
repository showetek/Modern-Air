//Lib
#include <SoftwareSerial.h>

//Var
int CO2Wert;              // CO2 Messwert in ppm

SoftwareSerial co2Serial(D2, D1); // RX, TX Pinns festlegen

//Funktion zum auslesen der Sensordaten
int leseCO2()                         
{
  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  char antwort[9];
  co2Serial.write(cmd, 9);
  co2Serial.readBytes(antwort, 9);
  if (antwort[0] != 0xFF) return -1;  // Bei fehlerhafter Verbindung Rückgabe von -1
  if (antwort[1] != 0x86) return -1;  //
  int antwortHigh = (int) antwort[2]; // CO2 High Byte
  int antwortLow = (int) antwort[3];  // CO2 Low Byte
  int ppm = (256 * antwortHigh) + antwortLow;
  return ppm;                         // Antwort des MH-Z19 CO2 Sensors in ppm
}

//Setup
void setup()
{            
  Serial.begin(9600);         //Serielle Verbindung zum Monitor
  co2Serial.begin(9600);      //Serielle Verbindung zum MH-Z19
  CO2Wert = leseCO2();        // MH-Z19 CO2 Sensor lesen
  Serial.println(CO2Wert);
}

//Mainloop des Hauptprogrammes
void loop()
{
  CO2Wert = leseCO2();       // MH-Z19 CO2 Sensor lesen
  Serial.println(CO2Wert);
  delay(5000);
}