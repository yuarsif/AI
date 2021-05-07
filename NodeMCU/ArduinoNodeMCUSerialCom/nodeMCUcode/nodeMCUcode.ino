#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino nodemcu
//D3      D3
//D4      D2
SoftwareSerial s(D2, D3);
int state =1;
float str = 1000;
float v;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  delay(1000);

}

void loop() {
      Serial.print("downlink receiving......");
      while(!s.available()>0){
        Serial.print("downlink receiving......fail");
       }
        if(s.available()>0){
          Serial.println(s.read());
          Serial.println("ok");
        }
        
        delay(100);
}
