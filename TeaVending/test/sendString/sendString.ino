#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino nodemcu
//D0      D3
//D1      D2
SoftwareSerial s(D3, D2);
int packets =0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(115200);
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT);
  delay(1000);
  
  DynamicJsonDocument doc(1024);

  String input =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
      
  deserializeJson(doc, input);
  
  JsonObject obj = doc.as<JsonObject>();
  
  Serial.println(obj);
  

  
  // No duplication is done.
  long time = obj[String("time")];


  obj[String("time")] = time;

  // You can get a String from a JsonObject or JsonArray:
  // No duplication is done, at least not in the JsonDocument.
  String sensor = obj["sensor"];
  //or
  sensor = obj["sensor"].as<String>();

  // You can set a String to a JsonObject or JsonArray:
  // WARNING: the content of the String will be duplicated in the JsonDocument.
  obj["sensor"] = sensor;

  // It works with serialized() too:
  obj["sensor"] = serialized(sensor);

  // You can also concatenate strings
  // WARNING: the content of the String will be duplicated in the JsonDocument.
  obj[String("sen") + "sor"] = String("gp") + "s";

  // You can compare the content of a JsonObject with a String
  if (obj["sensor"] == sensor) {
    // ...
  }

  // Lastly, you can print the resulting JSON to a String
  String output;
  serializeJson(doc, output);

    s.print(output);
}

void loop() {
  DynamicJsonDocument doc(1024);

  String input =
      "{\"sensor\":\"gps\",\"time\":1351824120,\"data\":[48.756080,2.302038]}";
      
  deserializeJson(doc, input);
  
  JsonObject obj = doc.as<JsonObject>();
  
  Serial.println(obj);

  String output;
  serializeJson(doc, output);
  Serial.println(output);
  s.println(output);
  delay(1000);
}
