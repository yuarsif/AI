//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino nodemcu
//D0      D3
//D1      D2

#include <SoftwareSerial.h>
SoftwareSerial s(1,0);
int packets=0;
int state=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);
  delay(1000);

}

void loop() {
    String teaApp;
    if(!s.available()>0){
        Serial.println("fail to read android app");
      }
    teaApp = s.read();
    const size_t capacity = 60;
    
  
   // Parse JSON object
    JsonObject root = jsonBuffer.parseObject(teaApp);
    if (!root.success()) {
      Serial.println(F("Parsing failed!"));
      return;
    }
  
    // Decode JSON and Extract values
    Serial.println(root["TEMPERATURE"].as<char*>());
    Serial.println(root["COIN"].as<char*>());
    Serial.println(root["LEVEL"][0].as<char*>());
    Serial.println(root["TIME"][1].as<char*>());
}
