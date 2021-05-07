//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino lolinwife
//D3      D3
//D4      D2
//Vin5v   Vin5v(use appropriate source of 5v)
//GND     GND

#include <SoftwareSerial.h>
SoftwareSerial s(4,3);
float sent = 10;
float v;
int count=0;
int state=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
delay(100);

for(int i=0; i<=20; i++){
    if(i==10&&state==0){
       
      while(!s.available()>0){
        Serial.println("downlink receiving......fail");
       }
       Serial.println(count);
       Serial.print("downlink str");
       v = s.parseFloat();
          if(s.read()=='\n'){
          Serial.print(v);
        }
        Serial.println("");
        delay(100);
      state =1;
    }
    if(i==20&&state==1){
      Serial.print("uplink sending.... ");
      Serial.print(sent);
      Serial.println(""); 
      s.print(sent);
      s.println("\n");
      sent++;
      delay(100);

      state =0;
    }
  }
  count++;
}
