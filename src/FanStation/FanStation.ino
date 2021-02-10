//Libs
#include <SoftwareSerial.h>

SoftwareSerial komm(4, 3);

int pwm = 0;
//pinMode(Output, 9);

void setup() {
 komm.begin(115200);
 Serial.begin(9600);

}


void loop() {
  
  pwm = komm.read();
   Serial.println(pwm);
   analogWrite(9,pwm);
   delay(5000);
}
