#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
extern "C" {
#include "user_interface.h"
}
#define PIXEL_PIN   D5    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 1
EspClass esp;
int CO2Wert;              // CO2 Messwert in ppm
uint32_t farbe = 0;
int Messinterval = 1;     // 1 Minute
 
SoftwareSerial co2Serial(D2, D1); // RX, TX Pinns festlegen
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
 
int leseCO2()                         // Kommunikation mit MH-Z19 CO2 Sensor
{
  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  char antwort[9];
  co2Serial.write(cmd, 9);
  co2Serial.readBytes(antwort, 9);
  if (antwort[0] != 0xFF) return -1;
  if (antwort[1] != 0x86) return -1;
  int antwortHigh = (int) antwort[2]; // CO2 High Byte
  int antwortLow = (int) antwort[3];  // CO2 Low Byte
  int ppm = (256 * antwortHigh) + antwortLow;
  return ppm;                         // Antwort des MH-Z19 CO2 Sensors in ppm
}
 
void setup()
{            
  Serial.begin(9600);
  co2Serial.begin(9600);
  CO2Wert = leseCO2();  
  Serial.println(CO2Wert);// MH-Z19 CO2 Sensor lesen
 
  
  

}
 
void loop()
{
 CO2Wert = leseCO2();
  Serial.println(CO2Wert);
  delay(5000);
}
 
