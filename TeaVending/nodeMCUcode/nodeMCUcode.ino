#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino nodemcu
//D0      D3
//D1      D2
SoftwareSerial s(D3, D2);
int packets =0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT);
  delay(1000);

}

void loop() {
int timer;
while(!s.available()>0){
        Serial.print("downlink receiving......fail");
       }
       
String vendingMachine ="{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
  deserializeJson(doc, input);
  JsonObject vendingMachine = doc.as<JsonObject>();

Serial.print("packets->");
Serial.print(packets);
Serial.println("");
Serial.print("uplink sending.... ");
Serial.print(vendingMachine);
Serial.println(""); 
s.print(vendingMachine);
packets++;

//receiving data from the arduino
  
//Serial.println(i);
//Serial.print("downlink receiving......");
//      
//v = s.parseFloat();
//if(s.read()=='\n'){
//Serial.println(v);
//}
//Serial.println("");

}
