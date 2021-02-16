#include <SoftwareSerial.h>   //using software serial so as not to conflict with serial download
SoftwareSerial mySerial(0, 1); // RX, TX

//Konst
const int fanPin = 9;          // Lüfter an Pin 9 angeschlossen
const int tachoPin = 2;        // Pin des Tachosignals des Lüfters

//Var
String fanSpeed = "";            // Variable für die Lüftergeschwindigkeit
int abfrZeit = 1000;           // Zeitabstand für die Abfragen des Tachosignals
long tachoMillis = abfrZeit;   // Variable zum speichern der Zeit
float rps = 0;                 // Variable mit Kommastelle für die Berechnung der Umdrehungen pro Sekunde
int rpm = 0;                   // Variable für die gemittelte Drehzahl
float umdrZeit = 0;            // Variable mit Kommastelle für die Zeit pro Umdrehung des Lüfters
float flankenZeit =0;          // Variable mit Kommastelle für die Zeit pro Puls des Lüfters
boolean pwm_check = false;
String pwm_ctrl = "";
String result_pwm_ctrl = "";
//Setup
void setup() {
  TCCR1B = TCCR1B & 0b11111000 | 0x01;    // Setzt Timer1 (Pin 9 und 10) auf 31000Hz
  mySerial.begin(115200); //setup software serial
  Serial.begin(115200);    //setup serial monitor
  pinMode(fanPin, OUTPUT);      // Setzt den Lüfter Pin als Ausgang
  pinMode(tachoPin, INPUT);     // Setzt den Tacho Pin als Eingang
}

void loop()  {
  result_pwm_ctrl = "";
  while (mySerial.available() == 0) {} 
  pwm_ctrl = mySerial.readString();
  //Serial.println(pwm_ctrl);
      
      for (int i =0; i < pwm_ctrl.length(); i++){
        
      if (pwm_ctrl.charAt(i) == '=') { 
        
        if (result_pwm_ctrl == "pwm") {
         pwm_check = true; 
         result_pwm_ctrl = "";
        }
        
      }
      if (pwm_ctrl.charAt(i) == ','){
        fanSpeed = result_pwm_ctrl;
        result_pwm_ctrl = "";
      }
      
      else{
        if (pwm_ctrl.charAt(i) != '=') {
        result_pwm_ctrl = result_pwm_ctrl + pwm_ctrl[i] ;
      }
      }
      }
  

//print received values to serial monitor
  //Serial.println(fanSpeed);
  if (pwm_check == true) {
  analogWrite(fanPin, fanSpeed.toInt());  // Gibt die Variable mit PWM aus
  }
  if((millis() - tachoMillis) >= abfrZeit)
  {     
    flankenZeit = pulseIn(tachoPin, LOW);    // Abfrage der Zeit pro Puls in Mikrosekunden
    umdrZeit = ((flankenZeit * 4)/1000);     // Berechnung der Zeit pro Umdrehung in Millisekunden
    rps = (1000/umdrZeit);                   // Umrechnung auf Umdrehungen pro Sekunde
    rpm = (rps*6);                           // Schritt 1 zur Rundung auf 10er Schritte der Drehzahl
    rpm = (rpm*10);   // Schritt 2 zur Rundung auf 10er Schritte der Drehzahl
    mySerial.flush();
    mySerial.end();
    mySerial.begin(115200);
    mySerial.print("rpm="+String(rpm)+",");
    Serial.println("rpm="+String(rpm)+",");                  // Ausgabe der Drehzahl im Seriellen Monitor
    tachoMillis = millis();      // Die TachoMillis werden aktualisiert um die nächsten 1000ms zählen zu können  
  }
  delay(3000);
}
