
#include <LiquidCrystal_I2C.h> // Library for LCD


LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);

//declear the sensor variable
int side = 7, aside = 6;
int sideValue, asideValue;
int state =0;
int count=0, block = 0;

void setup() {
  // Initiate the LCD:
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
  lcd.print("welcom ....!"); // Print the string "Hello World!"
  lcd.setCursor(0, 1); //Set the cursor on the third column and the second row (counting starts at 0!).
  lcd.print(" BLOCK  COUNTER ");
  delay(1000);
  
  //intialize count sensor
  pinMode(side, INPUT);
  pinMode(aside, INPUT);
}

void loop() {
  //read the counter input
  sideValue = digitalRead(side);
  asideValue = digitalRead(aside);

  
  if(sideValue ==  LOW && asideValue == LOW){
      count++;
      if(count == 100){
          block++;
          count = 0;
          if(block == 10){
            block=0;
//            sendMasege();
          }
        }
    }
  //print the passed block
  lcd.setCursor(0, 0);
  lcd.print("counter run...");
  lcd.setCursor(2, 1);
  lcd.print(count);
  lcd.print("BLOCKS");
}
