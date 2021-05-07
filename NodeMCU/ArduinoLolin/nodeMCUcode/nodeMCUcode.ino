#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino lolinwife
//D3      D3
//D4      D2
//Vin5v   Vin5v(use appropriate source of 5v)
//GND     GND
SoftwareSerial s(D3, D2);
int state =1;
float str = 1000;
float v;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT);
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:

  // put your main code here, to run repeatedly:
for(int i=0; i<=20; i++){
    if(i>10&&state==0){
      Serial.println(i);
      Serial.print("downlink receiving......");
      while(!s.available()>0){
        Serial.print("downlink receiving......fail");
       }
       v = s.parseFloat();
        if(s.read()=='\n'){
          Serial.println(v);
        }
        Serial.println("");
        delay(100);
      state =1;
    }
    if(i<10&&state==1){
      Serial.print("uplink sending.... ");
      Serial.print(str);
      Serial.println(""); 
      s.print(str);
      s.println("\n");
      str++;
      delay(100);

      state =0;
    }
  }
}
