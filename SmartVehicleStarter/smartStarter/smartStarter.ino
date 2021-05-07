#include<LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

#include <SoftwareSerial.h>
SoftwareSerial fp(5,6); // RX, TX
SoftwareSerial rf(4,3); // RX, TX



String rf_id="rf2020", rf_grant="NO";
String fp_id="pf2020", fp_grant="NO";
String state = "";
int engine = 7;
int starter = 2;
int change=0;
int pf_image = A1;
int rf_image = A0;
int fp_led=A3;
int rf_led=A4;
int starter_led=A5;


void setup()
{
  //intialize engine
  pinMode(engine, OUTPUT);
  pinMode(starter, INPUT);
  pinMode(engine, OUTPUT);
  pinMode(starter_led, OUTPUT);
  pinMode(rf_led, OUTPUT);
  pinMode(fp_led, OUTPUT);
  pinMode(rf_image, INPUT);
  pinMode(pf_image, INPUT);
  
  Serial.begin(9600);   //Baud rate for simulation purposes. A lot of actual GPS modules use a 4800 baud rate
  Serial.print("smart vehicle system "); 
  Serial.println("Test");
  Serial.println();

  //initialize module
  rf.begin(9600);
  fp.begin(9600);
  rf.print("smart vehicle system "); 
  rf.println("Test");
  rf.println(" ");
  fp.print("smart vehicle system "); 
  fp.println("Test");
  fp.println(" ");

  //initialize display
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("smart eng start");
  lcd.setCursor(0,1);
  lcd.print("    System      ");
  delay(2000);
  lcd.clear();
  
}

void loop()
{

  fp.println("finger image....");
  fp.println(rf_grant);
  rf.print("scane code....."); 
  rf.println(fp_grant);

  delay(1000);
  while (rf.available()>0)
  {

      (String)rf.read()="rf2020";
      rf.println(rf.read());
      if((String)rf.read()==rf_id);
      rf_grant = "OK";
      
      
  }
  while (rf.available()>0)
  {
      if((String)fp.read()==fp_id);
      fp_grant = "OK";
      
      
  }

  if(digitalRead(starter)==HIGH && digitalRead(rf_image)==HIGH && digitalRead(pf_image)==HIGH && change==0){
      fp_grant = "OK";
      rf_grant="OK";
      
      digitalWrite(rf_led, HIGH);
      digitalWrite(fp_led, HIGH);
      digitalWrite(starter_led, HIGH);
      delay(100);
      
      digitalWrite(rf_led, LOW);
      digitalWrite(fp_led, LOW);
      digitalWrite(starter_led, LOW);
      delay(1000);
      
      digitalWrite(rf_led, HIGH);
      digitalWrite(fp_led, HIGH);
      digitalWrite(starter_led, HIGH);
      delay(1000);
      
      digitalWrite(rf_led, LOW);
      digitalWrite(fp_led, LOW);
      digitalWrite(starter_led, LOW);
      delay(1000);
      change = 1;
    }
  if(digitalRead(starter)==LOW && digitalRead(rf_image)==LOW && digitalRead(pf_image)==LOW && change==1){
      fp_grant = "NO";
      rf_grant="NO";
      change =0;
    }
  
  if(fp_grant == "OK"  && rf_grant == "OK" && change == 1){
      state = "engine started";
      digitalWrite(engine, HIGH); 
      digitalWrite(rf_led, HIGH);
      digitalWrite(fp_led, HIGH);
      digitalWrite(starter_led, HIGH);   
    }
  if(fp_grant == "NO" && rf_grant=="NO" && change ==0){
    state = "off...";
    digitalWrite(engine, LOW);
    digitalWrite(rf_led, LOW);
    digitalWrite(fp_led, LOW);
    digitalWrite(starter_led, LOW);
    }
    lcd.setCursor(0,0);
    lcd.print("smart eng start");
    lcd.setCursor(1,1); 
    lcd.println(state);
    lcd.println(" grant");
    lcd.println(fp_grant);
 }
