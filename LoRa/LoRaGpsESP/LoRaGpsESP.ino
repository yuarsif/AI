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

//Credentials for Google GeoLocation API...
const char* Host = "www.googleapis.com";
String thisPage = "/geolocation/v1/geolocate?key=";
String key = "AIzaSyBgUYeKArvs_TUHTXoHJFDu8GZX_jEsiM4";

int status = WL_IDLE_STATUS;
String jsonString = "{\n";

double latitude    = 0.0;
double longitude   = 0.0;
double accuracy    = 0.0;
int more_text = 1; 
// set to 1 for more debug output

//define firebase
FirebaseData firebaseData;
FirebaseJson json;

//define gps
int RXPin = 0;
int TXPin = 5;
int GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

//define LoRa
#define ss 15
#define rst 16
#define dio0 2
int counter = 0;
int gate=0;
int gateway = 0;
int state_gate=0;
int state=0;
 
void setup() 
{
  Serial.begin(9600);
  Serial.println("LoRa Sender");

  
  LoRa.setPins(ss, rst, dio0);
    if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    delay(100);
    while (1);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa initializing OK....");
  delay(200);

  gpsSerial.begin(GPSBaud);
  Serial.print("connect to gps");
    while(!gpsSerial.available()>0){
      Serial.print(".___.");
      delay(300);
    }
  Serial.println("GPS initializing OK....");
  delay(200);

  Serial.println("connect to wife");  
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
  Serial.println("WiFe initializing OK....");  

  Serial.println("linking to firebase realtime database..."); 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);  
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  
  if(Firebase.get(firebaseData, path + "/gateway/state"))
  {
    //Success
    Serial.print("Get variant data success, type = ");
    Serial.println(firebaseData.dataType());

    if(firebaseData.dataType()=="int"){
      Serial.print("data = ");
      Serial.println(firebaseData.intData());
      while(gate > firebaseData.intData()){
          Serial.println("ERROR TO CONNECT TO GATEWAY");
        }

    }
  }

  delay(500);
}

int rand_data=0;
void loop() 
{

  if(Firebase.get(firebaseData, path + "/gateway/state"))
  {
    //Success
    Serial.print("Get variant data success, type = ");
    Serial.println(firebaseData.dataType());

    if(firebaseData.dataType()=="int"){
      Serial.print("data = ");
      Serial.println(firebaseData.intData());
      if(gate<firebaseData.intData()){
          gate = firebaseData.intData();
          state_gate = 1;
          Serial.println("OK");
        }
      else{
          Serial.println("ERROR TO CONNECT TO GATEWAY");
          state_gate =0;
        }

    }
  }
  
   StaticJsonBuffer<2000> jsonBuffer;
   float latitudes = gps.location.lat();
   float longitudes = gps.location.lng();
   float locations = gps.location.age();
   float altitudes = gps.altitude.meters();
   float courses = gps.course.deg();
   float speeds = gps.speed.kmph();
   
   JsonObject& root = jsonBuffer.createObject();
                root["latitude"] = latitudes;
                root["longitude"] = longitudes;
                root["location"] = locations;
                root["speed"] = speeds;
                root["altitude"] = altitudes;
                root["distance"] = courses;
                root["address"] = tracker_address;
                root["key"] = tracker_key;
                root["tracker"] = tracker_key;
                
                
  if (gps.location.isValid())
  {
   // send gps packet
    Serial.print("Sending gps packet: ");
    LoRa.beginPacket();
    root.printTo(LoRa);
    LoRa.endPacket();
    Serial.print("pgs packet sent: ");
  }
  Serial.print("packet: ");
  Serial.println(counter);
  Serial.print(F("Pkt No:"));
  Serial.print(counter);
  
  // send packet
  if(gpsSerial.available()>0){
  if(state_gate == 1){
      displayInfo();
    }
  Serial.println(" ");
  Serial.println(" ");
//  LoRa.beginPacket();
//  LoRa.print(counter);
//  root.printTo(LoRa);
//  LoRa.endPacket(); 
  counter++;
  }
  delay(500);
  
}

void displayInfo()
{
  if (gps.location.isValid())
  {
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
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/status", 2);
    
  }
  else
  {
    gpsbackup();    
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

  state++;
  delay(500);
}

void gpsbackup(){
  char bssid[6];
  DynamicJsonBuffer jsonBuffer;
  Serial.println("scan start");
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found...");

    if (more_text) {
      // Print out the formatted json...
      Serial.println("{");
      Serial.println("\"homeMobileCountryCode\": 234,");  // this is a real UK MCC
      Serial.println("\"homeMobileNetworkCode\": 27,");   // and a real UK MNC
      Serial.println("\"radioType\": \"gsm\",");          // for gsm
      Serial.println("\"carrier\": \"Vodafone\",");       // associated with Vodafone
      //Serial.println("\"cellTowers\": [");                // I'm not reporting any cell towers
      //Serial.println("],");
      Serial.println("\"wifiAccessPoints\": [");
      for (int i = 0; i < n; ++i)
      {
        Serial.println("{");
        Serial.print("\"macAddress\" : \"");
        Serial.print(WiFi.BSSIDstr(i));
        Serial.println("\",");
        Serial.print("\"signalStrength\": ");
        Serial.println(WiFi.RSSI(i));
        if (i < n - 1)
        {
          Serial.println("},");
        }
        else
        {
          Serial.println("}");
        }
      }
      Serial.println("]");
      Serial.println("}");
    }
    Serial.println(" ");
  }
  // now build the jsonString...
  jsonString = "{\n";
  jsonString += "\"homeMobileCountryCode\": 234,\n"; // this is a real UK MCC
  jsonString += "\"homeMobileNetworkCode\": 27,\n";  // and a real UK MNC
  jsonString += "\"radioType\": \"gsm\",\n";         // for gsm
  jsonString += "\"carrier\": \"Vodafone\",\n";      // associated with Vodacom Ltd
  jsonString += "\"wifiAccessPoints\": [\n";
  for (int j = 0; j < n; ++j)
  {
    jsonString += "{\n";
    jsonString += "\"macAddress\" : \"";
    jsonString += (WiFi.BSSIDstr(j));
    jsonString += "\",\n";
    jsonString += "\"signalStrength\": ";
    jsonString += WiFi.RSSI(j);
    jsonString += "\n";
    if (j < n - 1)
    {
      jsonString += "},\n";
    }
    else
    {
      jsonString += "}\n";
    }
  }
  jsonString += ("]\n");
  jsonString += ("}\n");
  //--------------------------------------------------------------------

  Serial.println("");

  WiFiClientSecure client;
  client.setInsecure();
  delay(500);

  //Connect to the client and make the api call
  Serial.print("Requesting URL: ");
  Serial.println("https://" + (String)Host + thisPage + "AIzaSyBgUYeKArvs_TUHTXoHJFDu8GZX_jEsiM4");
  Serial.println(" ");
  if (client.connect(Host, 443)) {
    Serial.println("Connected");
    client.println("POST " + thisPage + key + " HTTP/1.1");
    client.println("Host: " + (String)Host);
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println(jsonString.length());
    client.println();
    client.print(jsonString);
    delay(500);
  }

  //Read and parse all the lines of the reply from server
  while (client.available()) {
    String line = client.readStringUntil('\r');
    if (more_text) {
      Serial.print(line);
    }
    JsonObject& root = jsonBuffer.parseObject(line);
    if (root.success()) {
      latitude    = root["location"]["lat"];
      longitude   = root["location"]["lng"];
      accuracy   = root["accuracy"];
    }
  }

  Serial.println("closing connection");
  Serial.println();
  client.stop();

  Serial.print("Latitude = ");
  Serial.println(latitude, 6);
  Serial.print("Longitude = ");
  Serial.println(longitude, 6);
  Serial.print("Accuracy = ");
  Serial.println(accuracy);

   state =2;
    
    Serial.println("Location: GPS backup");
    
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/latitude", latitude);
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/longitude", longitude);
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/status", state);
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/tracker_address", tracker_address);
    Firebase.set(firebaseData, path + "/tracker/"+tracker+"/tracker_key", tracker_key);
      
  }
