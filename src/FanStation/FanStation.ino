#include <SoftwareSerial.h>   //using software serial so as not to conflict with serial download
SoftwareSerial mySerial(0, 1); // RX, TX

//Var
int pwm = 0;

//Setup
void setup() {

  mySerial.begin(115200); //setup software serial
  Serial.begin(115200);    //setup serial monitor
  pinMode(9,OUTPUT);
}

void loop()  {

  while (mySerial.available() == 0) { }
  pwm = mySerial.parseInt();
  

//print received values to serial monitor
  Serial.println(pwm);
  analogWrite(9,pwm);

  delay(100);
}
