//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino nodemcu
//D3      D3
//D4      D2

#include <SoftwareSerial.h>
SoftwareSerial s(4,3);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);
  delay(1000);

}
void loop() {

  //read heat
  float h = 70;
  // Read temperature
  float t = 37;
  // Read pressure
  float p = 170;
  //read sugar
  float g = 40;
      
      Serial.print("uplink sending.... ");
      Serial.println("");
      s.print(t);
      s.println("\n");      
      delay(300);
      
}
