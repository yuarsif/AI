

//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino nodemcu
//D3      D3
//D4      D2
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial s(2,3);
float i;
float v=0;
float temp, heart, pres, sugar;
int count=0;
int state=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);
  temp =27, heart=69, pres=120, sugar=3;
  delay(1000);

}

void loop() { 
      //sendig simple data to nodemcu
      
      Serial.print("downlinking.... ");
      
      //sending json multiple data to nodemcu
      StaticJsonBuffer<1000> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();
      
      root["temp"] = temp ;
      root["heart"] = heart;
      root["pres"] = pres;
      root["sugar"] = sugar;

      
      if(s.available()>0){

          //parse simple data from nodemcu
          Serial.print("uplinking.....");
          i = s.parseFloat();
          if(s.read()=='\n'){
          Serial.print(i);
          Serial.print(s.read());
          }
          Serial.println("");

          //print json multiple data to nodemcu
          root.printTo(s);
        
       }

       if(temp > 45){
          temp = 27;
        }
        if(heart > 90){
          heart = 65;
        }
        if(pres > 180){
          pres = 120;
        }
        if(sugar > 18){
          sugar = 5;
        }
        sugar++;
        pres++;
        heart++;
        temp++;

        delay(200);
       
}
