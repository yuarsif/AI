//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino nodemcu
//D3      D3
//D2      D2

#include <SoftwareSerial.h>
SoftwareSerial s(2,3);
float sent = 10;
float v;
int count=0;
int state=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);

}

void loop() {
  // put your main code here, to run repeatedly:

      while(!s.available()>0){
        Serial.println("fail nodemcu to  sending.... ");
        }
      Serial.println("node mcu data sending.... ");
      Serial.print(sent);
      Serial.println(""); 
      s.print(sent);
      s.println("\n");
      Serial.print(Serial.read());
      sent++;
      count++;
}
