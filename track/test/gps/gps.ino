#include <TinyGPS.h>

TinyGPS gps;  //Creates a new instance of the TinyGPS object




void setup()
{
  Serial.begin(9600);   //Baud rate for simulation purposes. A lot of actual GPS modules use a 4800 baud rate
  
  Serial.print("Simple TinyGPS library v. "); Serial.println(TinyGPS::library_version());
  Serial.println("Testing GPS");
  Serial.println();
}

void loop()
{
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      //Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;  //newData variable is set to true
    }
  }

  if (newData)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
    Serial.print("   Latitude = ");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);   //Parse the NMEA data to extract and display the latitude
    Serial.print("   Longitude = ");
    Serial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);  //Parse the NMEA data to extract and display the longitude

  }
  

  //This block checks if data coming in is fresh and to detect errors. 
  //This is not very necessary for a simulation but you can uncomment It if you want to see that data
  /*
  gps.stats(&chars, &sentences, &failed);
  Serial.print("      CHARS=");
  Serial.print(chars);
  Serial.print("      SENTENCES=");
  Serial.print(sentences);
  Serial.print("      CSUM ERR=");
  
  
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
    */
}
