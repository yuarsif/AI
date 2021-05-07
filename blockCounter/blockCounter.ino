
#include <LiquidCrystal_I2C.h> // Library for LCD
#include <SoftwareSerial.h>


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
SoftwareSerial mySerial(2,3);

//declear the sensor variable
int side = 7, aside = 6;
int sideValue, asideValue;
String msg="";
int state =0;
int count=0, block = 0;

void setup() {

  Serial.begin(9600);
  mySerial.begin(9600);
  // Initiate the LCD:
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
  lcd.print("welcom ....!"); // Print the string "Hello World!"
  lcd.setCursor(0, 1); //Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print(" BLOCK  COUNTER ");
  delay(3000);
  lcd.clear();
  
  //intialize count sensor
  pinMode(side, INPUT);
  pinMode(aside, INPUT);
}

void loop() {
  //read the counter input
  sideValue = digitalRead(side);
  asideValue = digitalRead(aside);


//  count state changer
  if(sideValue ==  LOW && asideValue == LOW && state == 0){
      count++;
      state =1;
      if(count%10==0){
          block++;
          msg =block+" batch sent";
        }
       else{
          msg = "";
        }
    }

     if(sideValue ==  HIGH && asideValue == HIGH && state == 1){
      
      state =0;
    }
    
  //print the passed block
  lcd.setCursor(0, 0);
  lcd.print("counter run...");
  lcd.setCursor(2, 1);
  lcd.print(msg);
  lcd.print(count);
  lcd.print("th  BLOCKS");
}

int sendMasege(int block, int count){
  Serial.println("Initializing..."); 
  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"+255787016111\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
  mySerial.println("BLOCK COUNTER SYSTEM"); //text content
  mySerial.print(block); //text content
  mySerial.println(" batch produced"); //text content
  updateSerial();
  mySerial.write(26);
  Serial.println("closing..."); 
  }

void updateSerial(){
  delay(500);
  while(!mySerial.available()>0){
      Serial.println("gsm not connected");
    }
  }
