#include <ArduinoJson.h>
#include <LiquidCrystal.h>
#include<SoftwareSerial.h>
#include "FirebaseESP8266.h"
#include <FirebaseJson.h>
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "vtsapp-2523.firebaseio.com"
#define FIREBASE_AUTH "GQXX50iOzT8L3y8sstwzKQsbylLeOldnudLR51d9"
#define WIFI_SSID "yuzar"
#define WIFI_PASSWORD "12345670"


//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseJson json;

void setup()
{
  Serial.begin(9600);
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

void loop(){
  
  String path="/vts";
  float lats =0;
  float longs =0;
  float distance =0;
  float spd =0;
  float overSpd =0;
  float state =0;
  String tracker_address = "192.168.153.40";
  String tracker_key="ZXXvMsjdGKL";
  String tracker ="RF480";

  int randdata = random(300);

  
 Serial.println("receiving...");

 if(randdata < 200){
      Serial.println("no connection to node");
      //6.8106722°S 39.2689194°

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
  String locat="6.8106722°S 39.2689194°";
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/location", locat);
  Serial.print(locat);
  Serial.println("");

  Serial.print("   speed ");
  spd=0.122254;
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/speed", spd);
  Serial.print(spd);
  Serial.println("");

  Serial.print("   status ");
  state = 2;
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/status", state);
  Serial.print(state);
  Serial.println("");

  Serial.print("   tracker address ");
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/tracker_address", tracker_address);
  Serial.print(tracker_address);
  Serial.println("");

  Serial.print("   tracker key ");
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/tracker_key", tracker_key);
  Serial.print(tracker_key);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");

  if(overSpd != 0){
  Serial.print("   overspeed ");
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/overspeed", overSpd);
  Serial.print(tracker_key);
  Serial.println("");
  Serial.println("---------------------xxxxx--------------------");
    }
    else{
      Firebase.set(firebaseData, path + "/tracker/"+tracker+"/overspeed", overSpd);
      }
  spd = spd+0.1;
  delay(100);
 }

 
  Serial.println("JSON not received and parsed");
  Serial.println("");
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/latitude", "00000000");
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/longitude", "00000000");
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/location", "0000000000000000");
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/speed", "00000000");
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/distance", "00000000");
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/tracker_address", tracker_address);
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/tracker_key", tracker_key);
  Firebase.set(firebaseData, path + "/tracker/"+tracker+"/status", 0);
  Serial.print(lats);
  
  delay(1000);
    
}
  
