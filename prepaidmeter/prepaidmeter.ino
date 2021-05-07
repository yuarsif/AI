#include <Wire.h>

/*
 * https://circuits4you.com
 * I2C LCD Interfacing with ESP8266 and ESP32
 * 
 */
#include <LiquidCrystal_I2C.h>
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display



#define FIREBASE_HOST "vtsapp-2523.firebaseio.com"
#define FIREBASE_AUTH "GQXX50iOzT8L3y8sstwzKQsbylLeOldnudLR51d9"
#define WIFI_SSID "ndende"
#define WIFI_PASSWORD "12345678"

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
String path="/meter";
String Path;
float cvol=0;
float bill=0;
float volbil=-1;
int totalvolbil;
int bilstate=0;


// Variable init
const int buttonPin = D2; // variable for D3 pin
const int relayPin = D7;
char push_data[200]; //string used to send info to the server ThingSpeak
int addr = 0; //endereço eeprom
byte sensorInterrupt = 0; // 0 = digital pin 2

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;


void ICACHE_RAM_ATTR flow ()                  // Interrupt function
{
  pulseCount++;
}

void setup()
{
 
    Serial.begin(115200); // Start the Serial communication to send messages to the computer
    delay(10);
    Wire.begin(0,5);
    Serial.println('\n');
    lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
    lcd.backlight();
    lcd.setCursor(2,0);
    lcd.print("Prepaid Meter!");
    lcd.setCursor(0,1);
    lcd.print("....DIT....");
    delay(2000);

    
    // Initialization of the variable “buttonPin” as INPUT (D2 pin)
    pinMode(buttonPin, INPUT);
  
    // Two types of blinking
    // 1: Connecting to Wifi
    // 2: Push data to the cloud
    pinMode(relayPin, OUTPUT); 
    
    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    oldTime = 0;

    digitalWrite(buttonPin, HIGH);
    attachInterrupt(digitalPinToInterrupt(buttonPin), flow, RISING);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  connecting  ");
  while (WiFi.status() != WL_CONNECTED)
  {
    lcd.setCursor(0, 1);
    lcd.print(".....");
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  Connected with IP:  ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
  Firebase.set(firebaseData, path + "/bilstate",0);
  Firebase.set(firebaseData, path + "/bil",0);
  delay(300);
  lcd.clear();
  
}
void loop() { 
      if(totalMilliLitres > volbil){
            lcd.clear();
            digitalWrite(relayPin, HIGH);
            lcd.setCursor(15, 0);
            Serial.println("bill must be paid");
            lcd.print("$");
            lcd.setCursor(2, 1);
            lcd.print("charge meter");
            delay(1000);

              if(Firebase.get(firebaseData, path + "/bilstate"))
        {
         //Success
          Serial.print("Get variant data success, type = ");
          Serial.println(firebaseData.dataType());

          if(firebaseData.dataType()=="int"){
          if(bilstate == firebaseData.intData()){
              if(Firebase.get(firebaseData, path + "/bil"))
              bill=firebaseData.intData();
              volbil = totalMilliLitres+(10*bill)+100;
              Serial.println("...........................................");
              Serial.print("bill");
              Serial.print(volbil);
              Serial.println("...........................................");
              Firebase.set(firebaseData, path + "/bilstate",1);
          }
        }
      }

            
            
            totalMilliLitres = 0;
            lcd.clear();
       }
       else{
       digitalWrite(relayPin, LOW);
       lcd.setCursor(0, 0);
       lcd.print("Rate");
       lcd.setCursor(5, 0);
       lcd.print(flowMilliLitres);
       lcd.setCursor(0, 1);
       lcd.print("Vol ");
       lcd.setCursor(5, 1);
       lcd.print(totalMilliLitres);
       delayMicroseconds(100);
       
        
        detachInterrupt(sensorInterrupt);

        detachInterrupt(sensorInterrupt);

        flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;

        oldTime = millis();
      
        flowMilliLitres = (flowRate / 60) * 1000;
        totalMilliLitres += flowMilliLitres;
        unsigned int frac;

        Serial.println("reate");
        Serial.print(flowMilliLitres);
        Serial.print("  volume  ");
        Serial.print(totalMilliLitres);

        
        if(Firebase.get(firebaseData, path + "/vol"))
        {
         //Success
          Serial.print("Get variant data success, type = ");
          Serial.println(firebaseData.dataType());

          if(firebaseData.dataType()=="int"){
          Serial.print("data = ");
          Serial.println(firebaseData.intData());
          Serial.println("OK");
          totalvolbil = firebaseData.intData();
        }
      }

           
        Path = path+"/rate";
        if(Firebase.setDouble(firebaseData, Path.c_str(), flowMilliLitres));
        
        Serial.print("  rate app ");
        Serial.print(Firebase.getDouble(firebaseData, Path.c_str()));

        
        Path = path+"/vol";
        
        Serial.print("  volume app");
        if(Firebase.setDouble(firebaseData, Path.c_str(), totalMilliLitres));
        Serial.println("");

        Path = path+"/voltotal";
        if(Firebase.setDouble(firebaseData, Path.c_str(), totalMilliLitres));
        
        
        }
        
}
