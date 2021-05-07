// LM35 temp sensor connected to analogue input A0, +5volt and ground

unsigned int total; // A/D readings
float tempC; // Celcius
float tempF; // Fahrenheit

void setup() {
  analogReference(INTERNAL); // use the internal ~1.1volt Aref | change to (INTERNAL1V1) for a Mega
  Serial.begin(9600);
}

void loop() {
  total = 0; // reset total
  for (int x = 0; x < 64; x++) { // 64(max) analogue readings for averaging
    total = total + analogRead(A3); // add each value
  }
  tempC = total * 0.001632; // Calibrate by changing the last digit(s)
  tempF = tempC * 1.8 + 32; // Celcius to Fahrenheit

  Serial.print("The temperature is  ");
  Serial.print(tempC, 1); // one decimal place
  Serial.print(" Celcius  ");
  Serial.print(tempF, 1); // one decimal place
  Serial.println(" Fahrenheit");

  delay(1000); // use a non-blocking delay when combined with other code
}
