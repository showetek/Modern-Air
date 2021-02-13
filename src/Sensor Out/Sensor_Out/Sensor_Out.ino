//Lib
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

//Define
#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "PASS"
#endif

//Const Var
const char* ssid     = STASSID;
const char* password = STAPSK;

//Var
int CO2Wert;              // CO2 Messwert in ppm
int tmpWert;              // tmp Messwert in °C

SoftwareSerial co2Serial(D2, D1); // RX, TX Pinns festlegen

//Funktion zum auslesen des CO2s
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

//Funktion zum auslesen der Temperatur
int lesetmp(){
  byte cmd[9] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
  char antwort[9];
  co2Serial.write(cmd, 9);
  co2Serial.readBytes(antwort, 9);
  if (antwort[0] != 0xFF) return -1;  // Bei fehlerhafter Verbindung Rückgabe von -1
  if (antwort[1] != 0x86) return -1;  //
  int tmp = antwort[4] - 40;          // Tmp Berechnung mit 4. Datensatz
  return tmp;                         // Antwort des MH-Z19 CO2 Sensors in °C
}

void post(String key1, String key2){
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin("http://192.168.178.75:80/postform/");      //Specify request destination
    http.addHeader("Content-Type", "text/plain");  //Specify content-type header
 
    int httpCode = http.POST("CO2_out=" + key1 + "," + key2 +",");   //Send the request
    String payload = http.getString();                  //Get the response payload
 
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
}

//Setup
void setup()
{            
    Serial.begin(9600);         //Serielle Verbindung mit Monitor
  WiFi.begin(ssid, password); //WIFI Verbinden
  Serial.println("");

  //Auf Verbindung warten
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  
  co2Serial.begin(9600);      //Serielle Verbindung zum MH-Z19
  CO2Wert = leseCO2();        // MH-Z19 CO2 Sensor lesen
  tmpWert = lesetmp();        // MH-Z19 tmp Sensor lesen
  Serial.println(CO2Wert);
  Serial.println(tmpWert);
}

//Mainloop des Hauptprogrammes
void loop()
{
  CO2Wert = leseCO2();       // MH-Z19 CO2 Sensor lesen
  tmpWert = lesetmp();
  post(String(leseCO2()), String(lesetmp()));
  Serial.println(CO2Wert);
  Serial.println(tmpWert);
  delay(5000);
}
