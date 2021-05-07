#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial s(D3, D2);

#define FIREBASE_HOST "vtsapp-2523.firebaseio.com"
#define FIREBASE_AUTH "GQXX50iOzT8L3y8sstwzKQsbylLeOldnudLR51d9"
#define WIFI_SSID "SAKURA_FTE1"
#define WIFI_PASSWORD "diveleclasic"


//Define FirebaseESP8266 data object
FirebaseData firebaseData;


  String path="/meter";
  float rate;
  float vol;
  
void setup() {
  Serial.begin(9600);
  s.begin(4800);
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT);
  delay(1000);

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
  delay(300);
}
 
void loop() {
  getjson();
  if(s.available()>0)
    {
      Serial.println("OK...");
    }
    else{
       Serial.println("meter is not available");
      }
}

void getjson(){
  Serial.println("JSON received and parsed");

  Serial.print(" Rate ");
  Serial.println("");
  Firebase.set(firebaseData, path +"/rate", rate);
  Serial.print(rate);
  
  Serial.print("   Volume ");
  Serial.print(vol);
  Firebase.set(firebaseData, path +"/volume", vol);
  Serial.println("");
  
  }
