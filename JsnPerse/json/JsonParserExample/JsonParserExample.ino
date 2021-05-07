#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(2,3);
 
void setup() {
    Serial.begin(9600);
    s.begin(9600);
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
      root.printTo(s);
    }
    root.printTo(Serial);

    
    
    
    
    
    
    //passing json from done
  JsonObject& root2 = jsonBuffer.parseObject(Serial);
  if (root2 == JsonObject::invalid())
    return;
 
  Serial.println("JSON received and parsed");
  
  Serial.print("Data 1 ");
  Serial.println("");
  int data1=root2["data1"];
  Serial.print(data1);
  Serial.print("   Data 2 ");
  int data2=root2["data2"];
  Serial.print(data2);
  Serial.println("");
  Serial.print("   Data 3 ");
  int data3=root2["data3"];
  Serial.print(data3);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
}
