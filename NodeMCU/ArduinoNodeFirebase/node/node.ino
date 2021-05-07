#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial s(2,3);
void setup() {
  Serial.begin(9600);
  s.begin(4800);
  delay(1000);
  
}
 
void loop() {
 StaticJsonBuffer<1000> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
 
  root["latitude"] = 100;
  root["longitude"] = 200;
  root["distance"] = 300;
  root["location"] = 400;
  root["speed"] = 500;
  root["overspeed"] = 600;
  root["state"] = 0;
  root["tracker"] = "RF408";
  
  if(s.available()>0)
    {
      Serial.println("OK...");
      root.printTo(s);
    }
}
