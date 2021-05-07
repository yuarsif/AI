#include "FirebaseESP8266.h"
#include <FirebaseJson.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

SoftwareSerial s(D2, D3);

#define FIREBASE_HOST "vtsapp-2523.firebaseio.com"
#define FIREBASE_AUTH "GQXX50iOzT8L3y8sstwzKQsbylLeOldnudLR51d9"
#define WIFI_SSID "yuzar"
#define WIFI_PASSWORD "12345670"


//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseJson json;
 
void setup() {
  Serial.begin(9600);
  s.begin(4800);
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}
 
void loop() {
  String path="/vts";
  float lats =0;
  float longs =0;
  float distance =0;
  float locat =0;
  float spd =0;
  float overSpd =0;
  float state =0;
  String tracker ="RF408";

  
  Serial.print("receiving...");
  while(!s.available()>0){
      Serial.println("no connection to node");
    }


//    receiving data from node
//    passing json from done
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
    return;
 
  Serial.println("JSON received and parsed");
  root.prettyPrintTo(Serial);
  
  Serial.print(" Latitudes ");
  Serial.println("");
  lats=root["latitude"];
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/latitude", lats);
  Serial.print(lats);
  
  Serial.print("   Longitude ");
  longs=root["longitude"];
  Serial.print(longs);
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/longitude", longs);
  Serial.println("");
  
  Serial.print("   Distance ");
  distance=root["distance"];
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/distance", distance);
  Serial.print(distance);
  Serial.println("");

  Serial.print("   location ");
  locat=root["location"];
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/location", locat);
  Serial.print(locat);
  Serial.println("");

  Serial.print("   speed ");
  spd=root["speed"];
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/speed", spd);
  Serial.print(spd);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
  
  delay(200);
}
