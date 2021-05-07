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

//Default baud of NEO-6M is 9600
int GPSBaud = 9600;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

void setup()
{
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
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
  // Displays information when new sentence is available.
  
  while (gpsSerial.available() > 0)
    Serial.write(gpsSerial.read());
    json.clear().add("data",gpsSerial.read());
    Firebase.pushJSON(firebaseData, "vts/tracker/nime/test", json);
}

//reference https://lastminuteengineers.com/neo6m-gps-arduino-tutorial/
