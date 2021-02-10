//Libs
#include <SoftwareSerial.h>

//SoftwareSerial
SoftwareSerial komm(4, 3);

//Var
int pwm = 0;

//Setup
void setup() {
 komm.begin(115200);
 Serial.begin(9600);
}

//Mainloop des Hauptprogrammes
void loop() {
   pwm = komm.read();
   Serial.println(pwm);
   analogWrite(9,pwm);
   delay(5000);
}