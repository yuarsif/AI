//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino nodemcu
//D2      D3
//D3      D2
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial s(3, 4);
void setup() {
  Serial.begin(9600);
  s.begin(115200);
}

void loop() {
    while (!Serial.available()>0){
      Serial.println("no connection");
      }
    // Print values.
    s.println(Serial.read());
    delay(300);
}
