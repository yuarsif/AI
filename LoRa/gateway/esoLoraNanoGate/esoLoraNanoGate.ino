#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>

SoftwareSerial s(3,4);

void setup() {
  Serial.begin(9600);
  s.begin(9600);
  
  while (!Serial);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  Serial.println("LoRa Receiver");

  while(!s.available()>0){
      Serial.println("connection fail to node mcu");
    }

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
   StaticJsonBuffer<2000> jsonBuffer;
   float state = 0;
   
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
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
                root.printTo(s);
  

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
  
  else{
  JsonObject& root = jsonBuffer.createObject();
                root["status"] = state;
                root.printTo(s);
  
  }
  
}
