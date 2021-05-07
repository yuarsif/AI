#include "FirebaseESP8266.h"
#include <FirebaseJson.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino nodemcu
//D3      D3
//D4      D2
SoftwareSerial s(D3, D2);
int count=0;
int state=0;
float i = 0;
float v;

#define FIREBASE_HOST "appfirebase-900900.firebaseio.com"
#define FIREBASE_AUTH "xZgL8v89fE75mhQlMzkcbo9L1aZQzrwcL8vYSwR9"
#define WIFI_SSID "yuzar"
#define WIFI_PASSWORD "12345670"


//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseJson json;


String path="/test_measure";
float temp=24; 
float pressure;
float sugar;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
  delay(1000);

}

void loop() {

  if(temp > 39){
          sugar = 100;
        }        

       
       if(s.available()>0){
          Serial.print("uplinking heart rate measurement.....");
          float heart = s.parseFloat();
          
          if(heart-60>0){
             pressure = 100+(heart-60);
            }
          if(s.read()=='\n'){
          if(heart<100&&heart>60){          
            Firebase.set(firebaseData, path +"/heart", heart);
            Firebase.set(firebaseData, path +"/temperature", sugar);
            Firebase.set(firebaseData, path +"/pressure", pressure);
            Firebase.set(firebaseData, path +"/glucose", temp);
            
          }

          
          if(temp<40){
            temp++;
         }
          
          
          Serial.print(heart);
          Serial.print(s.read());
          }
          count++;
       }
        count++;
      Serial.print("uplink sending.... ");
      Serial.print(i);
      Serial.println(""); 
      s.print(i);
      s.println("\n");
      i--;
      delay(100);
}

  void gettest(){
  StaticJsonBuffer<1200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(s);
  if (root == JsonObject::invalid())
    return;
 
  Serial.println("JSON received and parsed");
  Serial.print(" measure parameter ");
  Serial.println("");
  float temp=root["temp"];
  if(temp==0){}
  else{Firebase.set(firebaseData, path +"/temperature", temp);}
  Serial.println(" temperature :");
  Serial.print(temp);
  float heart=root["heart"];
  if(heart==0){}
  else{Firebase.set(firebaseData, path +"/heart", heart);}
  Serial.println(" heart beaty :");
  Serial.print(heart);
  float pres=root["pres"];
  if(pres==0){}
  else{Firebase.set(firebaseData, path +"/pressure", pres);}
  Serial.println(" blood pressure :");
  Serial.print(pres);
  float sugar=root["sugar"];
  if(pres==0){}
  else{Firebase.set(firebaseData, path +"/sugar", sugar);}
  Serial.println(" blood sugar :");
  Serial.print(sugar);
  delay(1000);
  
  
  }
