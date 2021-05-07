#include <TinyGPS++.h>
#include <SoftwareSerial.h>
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

// Choose two Arduino pins to use for software serial
int RXPin = 5;
int TXPin = 4;

int GPSBaud = 9600;

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);
  gpsSerial.begin(GPSBaud);

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

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      displayInfo();

  
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
}

void displayInfo()
{
  if (gps.location.isValid())
  {
  String path="/vts";
  String tracker_address = "192.168.153.40";
  String tracker_key="ZXXvMsjdGKL";
  String tracker ="RF480";
    Serial.print("Latitude: ");
    float latitudes = gps.location.lat();
    Serial.println(latitudes, 6);
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/latitude", latitudes);
    Serial.print("Longitude: ");
    float longitudes = gps.location.lng();
    Serial.println(longitudes, 6);
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/longitude", longitudes);
    Serial.print("location: ");
    float locations = gps.location.age();
    Serial.println(gps.location.age(), 6);
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/location", locations);
    Serial.print("altitude: ");
    Serial.println(gps.altitude.meters());
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/altitude", gps.altitude.meters());
    Serial.print("course: ");
    Serial.println(gps.course.deg());
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/distance", gps.course.deg());
    Serial.print("speed: ");
    Serial.println(gps.speed.kmph());
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/speed", gps.speed.kmph());
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/tracker_address", tracker_address);
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/tracker_key", tracker_key);
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/status", 0);
    
  }
  else
  {
    Serial.println("Location: Not Available");
  }
  
  Serial.print("Date: ");
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.println(gps.date.year());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.print("Time: ");
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(":");
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(":");
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(".");
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gps.time.centisecond());
  }
  else
  {
    Serial.println("Not Available");
  }

  Serial.println();
  Serial.println();
  delay(1000);
}

//reference https://lastminuteengineers.com/neo6m-gps-arduino-tutorial/
