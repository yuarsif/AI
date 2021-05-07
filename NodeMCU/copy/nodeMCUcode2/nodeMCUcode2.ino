#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino nodemcu
//D3      D3
//D4      D2
SoftwareSerial s(D3, D2);
int state =1,i=0;
float str = 1000;
float v;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  delay(1000);

}

void loop() {
 
      Serial.print("downlink receiving......");
       v = s.parseFloat();
        if(s.read()=='\n'){
          Serial.println(v);
        }
        Serial.println("");
        state =1;
  
}
