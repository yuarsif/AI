#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>
#include<SoftwareSerial.h>
#include "FirebaseESP8266.h"
#include <FirebaseJson.h>
#include <ESP8266WiFi.h>

//define WiFe
#define FIREBASE_HOST "vtsapp-2523.firebaseio.com"
#define FIREBASE_AUTH "GQXX50iOzT8L3y8sstwzKQsbylLeOldnudLR51d9"
#define WIFI_SSID "yuzar"
#define WIFI_PASSWORD "12345670"

//define Tracker
String path="/vts";
String tracker_address = "192.168.153.430";
String tracker_key="ZXXvMsjdGKL";
String tracker ="RF408";

//define firebase
FirebaseData firebaseData;
FirebaseJson json;

SoftwareSerial s(D3, D2);
int count=0;
float i = 0;
float v;
int state =0;

//define WiFe
#define FIREBASE_HOST "vtsapp-2523.firebaseio.com"
#define FIREBASE_AUTH "GQXX50iOzT8L3y8sstwzKQsbylLeOldnudLR51d9"
#define WIFI_SSID "yuzar"
#define WIFI_PASSWORD "12345670"

 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }

  
//  while(s.available()>0){
//      Serial.println("connection fail to node mcu");
//    }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

//  if (LoRa.begin(433E6)) {
//    Serial.println("Starting LoRa failed!");
//    while (1);
//  }
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  delay(1000);

}

void loop() {
      
      delay(100);

      if(count > 20){
        state =2;
        Firebase.set(firebaseData, path + "/tracker/"+tracker+"/status", state );
        }
        else{
          state =0;
          
          Firebase.set(firebaseData, path + "/tracker/"+tracker+"/status", state );
          }
          Serial.println(state);
        count++;
        
}

  void getgps(){
  StaticJsonBuffer<1200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
    return;
 
  Serial.println("JSON received and parsed");
  Serial.print(" measure parameter ");
  Serial.println("");
  int state =root["status"];
  if(state == 0){}
  else{Firebase.set(firebaseData, path + "/tracker/"+tracker+"/status", state );}
  Serial.println(" status :");
  Serial.print(state);
  delay(500);
  
  }
