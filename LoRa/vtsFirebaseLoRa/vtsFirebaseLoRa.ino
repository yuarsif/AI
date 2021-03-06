                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    //FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"
#include <FirebaseJson.h>
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "vtsapp-2523.firebaseio.com"
#define FIREBASE_AUTH "GQXX50iOzT8L3y8sstwzKQsbylLeOldnudLR51d9"
#define WIFI_SSID "yuzar"
#define WIFI_PASSWORD "12345670"

#include <SPI.h>
#include <LoRa.h>

#define ss 15
#define rst 16
#define dio0 2

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseJson json;



void setup()
{
  Serial.begin(9600);
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);

  //LoRa connection to the node
  Serial.println("LoRa Receiver Callback");
 
  LoRa.setPins(ss, rst, dio0);
 
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  //wife connect to internet
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("Connecting to Wi-Fi");
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

void loop(){
  
  String path="/vts";
  float lats =0;
  float longs =0;
  float distance =0;
  float locat =0;
  float spd =0;
  float overSpd =0;
  float state =0;
  String tracker ="RF408";

  int randdata = random(300);

  
  Serial.print("receiving from node ...");
  int packetSize = LoRa.parsePacket();
  if(packetSize){
    Serial.print("gateway send to app");
    while (LoRa.available()) {
      
      if(randdata < 200){
      Serial.println("ok");
      //6.8106722??S 39.2689194??

        Serial.print(" Latitudes ");
        Serial.println("");
        lats=6.8106722;
        Firebase.set(firebaseData, path + "/tracker/"+tracker+"/latitude", lats);
        Serial.print(lats);
  
        Serial.print("   Longitude ");
        longs=39.2689194;
        Serial.print(longs);
        Firebase.set(firebaseData, path + "/tracker/"+tracker+"/longitude", longs);
        Serial.println("");
  
        Serial.print("   Distance ");
        distance=0;
        Firebase.set(firebaseData, path + "/tracker/"+tracker+"/distance", distance);
        Serial.print(distance);
        Serial.println("");

        Serial.print("   location ");
        String locat="6.8106722??S 39.2689194??";
        Firebase.set(firebaseData, path + "/tracker/"+tracker+"/location", locat);
        Serial.print(locat);
        Serial.println("");

        Serial.print("   speed ");
        spd=0;
        Firebase.set(firebaseData, path + "/tracker/"+tracker+"/speed", spd);
        Serial.print(spd);
        Serial.println("");
        Serial.println("---------------------xxxxx--------------------");
  
        delay(200);
 }
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    }
 
  Serial.println("JSON received and parsed");
  
  Serial.print(" Latitudes ");
  Serial.println("");
  lats=0;
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/latitude", lats);
  Serial.print(lats);
  
  Serial.print("   Longitude ");
  longs=0;
  Serial.print(longs);
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/longitude", longs);
  Serial.println("");
  
  Serial.print("   Distance ");
  distance=0;
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/distance", distance);
  Serial.print(distance);
  Serial.println("");

  Serial.print("   location ");
  locat=0;
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/location", locat);
  Serial.print(locat);
  Serial.println("");

  Serial.print("   speed ");
  spd=0;
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/speed", spd);
  Serial.print(spd);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
  
  delay(200);
    
}
  
