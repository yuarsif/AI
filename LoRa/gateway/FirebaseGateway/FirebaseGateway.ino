#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <FirebaseJson.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
//define WiFe
#define FIREBASE_HOST "vtsapp-2523.firebaseio.com"
#define FIREBASE_AUTH "GQXX50iOzT8L3y8sstwzKQsbylLeOldnudLR51d9"
#define WIFI_SSID "yuzar"
#define WIFI_PASSWORD "12345670"


//2. Define FirebaseESP8266 data object for data sending and receiving
FirebaseData firebaseData;
int gate =0;
int state =0;
#define ss 15
#define rst 16
#define dio0 2
#define Lora_Led D1
#define Gps_Led D3

//Credentials for Google GeoLocation API...
const char* Host = "www.googleapis.com";
String thisPage = "/geolocation/v1/geolocate?key=";
String key = "AIzaSyBgUYeKArvs_TUHTXoHJFDu8GZX_jEsiM4";
unsigned long startMillis;
unsigned long startMillisL;
unsigned long startMillisG;
unsigned long currentMillis;
unsigned long currentMillisL;
unsigned long currentMillisG;

int status = WL_IDLE_STATUS;
String jsonString = "{\n";

double latitude    = 0.0;
double longitude   = 0.0;
double accuracy    = 0.0;
int more_text = 1; // set to 1 for more debug output
void setup()
{

  Serial.begin(115200);
  pinMode(Lora_Led,OUTPUT);
  pinMode(Gps_Led,OUTPUT);

  delay(100);
  LoRa.setPins(ss, rst, dio0);
    if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    delay(100);
    while (1);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa initializing OK....");
  delay(100);

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

  //3. Set your Firebase info

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}
int i=0;
void loop()
{
  
  String path ="/vts/gateway/state";
  if(Firebase.get(firebaseData, path))
  {
    //Success
    Serial.print("Get variant data success, type = ");
    Serial.println(firebaseData.dataType());

    if(firebaseData.dataType()=="int"){
      Serial.print("data = ");
      Serial.println(firebaseData.intData());
      if(gate<firebaseData.intData()){
          gate = firebaseData.intData();
          Firebase.setInt(firebaseData, path, gate+1);
          if(gate%20==0){
            Firebase.set(firebaseData, "/vts/tracker/RF408/status", 0);
            
            }
        }
      else{
          Serial.println("ERROR");
        }

    }
    i++;
  }
  else{
    //Failed?, get the error reason from firebaseData

    Serial.print("Error in get, ");
    Serial.println(firebaseData.errorReason());
  }

  digitalWrite(Lora_Led,HIGH);  
  digitalWrite(Gps_Led,HIGH); 
  delay(500);
  digitalWrite(Gps_Led,LOW); 

  
}

void localGateway(){
  String path="/vts";
  String tracker_address = "192.168.153.430";
  String tracker_key="ZXXvMsjdGKL";
  String tracker ="RF408";
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

  if(Firebase.get(firebaseData, "/vts/tracker/"+tracker+"/latitude")){

    float lat1= firebaseData.floatData();
    Serial.println(lat1);
    
    }
  if(Firebase.get(firebaseData, "/vts/tracker/"+tracker+"/longitude")){

    
    float long1= firebaseData.floatData();
    Serial.println(long1);
    
    }

    
  }
