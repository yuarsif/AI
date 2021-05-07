#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(7,OUTPUT);
  pinMode(6,OUTPUT);
  Serial.println("LoRa Receiver");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  
  int loraData;
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    digitalWrite(7,HIGH);
    

    delay(100);
    digitalWrite(7,LOW);
   
  
    loraData = LoRa.read();

    if(loraData>20){digitalWrite(6,HIGH);}
    if(loraData>30){digitalWrite(6,LOW);}
    while (LoRa.available()) {
      Serial.println((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }

  
}
