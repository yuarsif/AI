#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial s(3,4);

int state=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);
  
  while(!s.available()>0){
      Serial.println("connection fail to node mcu");
    }

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  delay(1000);

}

void loop() { 
      //sendig simple data to nodemcu
    StaticJsonBuffer<2000> jsonBuffer;
    Serial.print("downlinking.... ");
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
    Serial.print("Received packet '");
    digitalWrite(7,HIGH);
    delay(100);
    digitalWrite(7,LOW);
    while (LoRa.available()) {
      Serial.println((char)LoRa.read());
    }

    JsonObject& root = jsonBuffer.createObject();
                state =2;
                root["status"] = state;

                if(s.available()>0){
                  root.printTo(s);
                  }
  

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  else{
      JsonObject& root = jsonBuffer.createObject();
                root["status"] = state;

                if(s.available()>0){
                  root.printTo(s);
                  }
    }

  delay(200);
       
}
