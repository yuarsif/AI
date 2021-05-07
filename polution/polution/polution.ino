//Include the library
#include <MQUnifiedsensor.h>
#include <LiquidCrystal.h> 

/************************Hardware Related Macros************************************/
#define         Board                   ("Arduino UNO")
#define         Pin                     (A3)  //Analog input 3 of your arduino
/***********************Software Related Macros************************************/
#define         Type                    ("MQ-3") //MQ3
#define         Voltage_Resolution      (5)
#define         DoGas 3
#define         ADC_Bit_Resolution      (10) // For arduino UNO/MEGA/NANO
#define         RatioMQ3CleanAir        (60) //RS / R0 = 60 ppm 
/*****************************Globals***********************************************/
//delay declaration
int period = 1000;
unsigned long time_now = 0;

//LCD setup
int Contrast=13;
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

//Declare Sensor
MQUnifiedsensor MQ3(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);


//air clear setup
int cleaner = 4, cleanerState = 0;

void setup() {
  //Init the serial port communication - to debug the library
  Serial.begin(9600);
  pinMode(DoGas, INPUT);
  pinMode(cleaner, OUTPUT);

  //initialize LCD
  pinMode(Contrast, OUTPUT);
  lcd.begin(16, 2);
  digitalWrite(Contrast, HIGH);
  delay(10);
  digitalWrite(Contrast, LOW);
  delay(10);
  lcd.setCursor(0, 0);
  lcd.print("welcome....!");
   
  lcd.setCursor(0, 1);
  lcd.print("polution system");

  //Set math model to calculate the PPM concentration and the value of constants
  MQ3.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ3.setA(521853); MQ3.setB(-3.821);
  /*
    Exponential regression:
  Gas    | a      | b
  LPG    | 44771  | -3.245
  CH4    | 2*10^31| 19.01
  CO     | 521853 | -3.821
  Alcohol| 0.3934 | -1.504
  Benzene| 4.8387 | -2.68
  Hexane | 7585.3 | -2.849
  */

  /*****************************  MQ Init ********************************************/ 
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ3.init(); 
  
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ3.update();
    calcR0 += MQ3.calibrate(RatioMQ3CleanAir);
    Serial.print(".");
  }
  MQ3.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue founded"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 
  MQ3.serialDebug(true);
}

void loop() {
  String gasValue = " ";
  float gasPercent;
   if(millis() >= time_now + period){
        time_now += period;
        MQ3.update(); 
        MQ3.readSensor();
        gasPercent = MQ3.readSensor();
        MQ3.serialDebug();
    }
    
  if(gasPercent > 0.20 && cleanerState == 0){
    digitalWrite(cleaner, HIGH);
    gasValue = "dirt";
    cleanerState = 1;
    }
    
  if(gasPercent < 0.20 && cleanerState == 1){
    gasValue ="clear";
    digitalWrite(cleaner, LOW);
    cleanerState = 0;
    }


  digitalWrite(Contrast, HIGH);
  delay(10);
  digitalWrite(Contrast, LOW);
  delay(10);
  lcd.setCursor(0, 0);
  lcd.print("condition "); 
  lcd.print(gasValue); 
  lcd.setCursor(0, 1);
  lcd.print(gasPercent);
  lcd.print(" %polution");
}
