#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial s(4, 5);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(9600);
  delay(1000);

  while(!s.available()>0){
    Serial.println("connection to arduino fail");
    }

}

void loop() {
      while(!s.available()>0){
        Serial.print("downlink receiving......fail");
       }
        if(s.available()>0){
          Serial.println(s.read());
          Serial.println("ok");
        }
        
        delay(100);
}
