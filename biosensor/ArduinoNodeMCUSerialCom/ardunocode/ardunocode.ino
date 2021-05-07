int sensor_pin = A2;                

int led_pin = A5;                  

volatile int heart_rate;          

volatile int analog_data;              

volatile int time_between_beats = 600;            

volatile boolean pulse_signal = false;    

volatile int beat[10];         //heartbeat values will be sotred in this array    

volatile int peak_value = 512;          

volatile int trough_value = 512;        

volatile int thresh = 525;              

volatile int amplitude = 100;                 

volatile boolean first_heartpulse = true;      

volatile boolean second_heartpulse = false;    

volatile unsigned long samplecounter = 0;   //This counter will tell us the pulse timing

volatile unsigned long lastBeatTime = 0;

//Set the pin of arduino as defined do not change 
//otherwise you know what you are doing
//arduino nodemcu
//D3      D3
//D4      D2
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#define esp 4
#define wife 5
#define temp_led 6
#define heart_led 7
SoftwareSerial s(2,3);
int temperature = 27;

float i;
float v=0;
float pres, sugar;
int count=0;
int state=0;
int state2=0;
float heart=0;
float temp=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s.begin(4800);
  pres=120;  sugar=3;
  pinMode(led_pin,OUTPUT); 
  pinMode(esp, OUTPUT);
  pinMode(wife,OUTPUT);
  pinMode(temp_led,OUTPUT);
  pinMode(heart_led,OUTPUT);
  interruptSetup();  
}

void loop() { 
      //sendig simple data to nodemcu
      temp = analogRead(A3); // add each value
      float pressure;
      float sugar;
      Serial.print("downlinking.... ");
      Serial.print("BPM: ");
      Serial.println(heart_rate);

      Serial.print("Temp - ");  //Send a text to serial
      Serial.print(temp); //It gets the temperature in celcius and send to serial
      
      //sending json multiple data to nodemcu
      StaticJsonBuffer<1200> jsonBuffer;
      JsonObject& root = jsonBuffer.createObject();


      root["heart"] = heart;
      root["temp"]=temp;
      root["pres"] = pres;
      root["sugar"] = sugar;

      if(temperature > 39){
          sugar = 100;
        }
      if(heart_rate < 80&& temperature <37){
        sugar = 50;
        }
        
       if(heart_rate-60>0){
          pressure = 100+(heart_rate-60);
        }
       
      
      String biosensor =(String)heart_rate+";"+temperature+";"+pressure+";"+sugar;
      if(s.available()>0){

          

          //parse simple data from nodemcu
          Serial.print("uplinking.....");
          i = s.parseFloat();
          if(s.read()=='\n'){
          Serial.print(i);
          Serial.print(s.read());
          
          }

          Serial.print("uplink sending.... ");
          if(heart_rate<150){
          Serial.print(heart_rate);
          Serial.println(""); 
          s.print(biosensor);
          s.println("\n");
          Serial.println("");
          }

          //print json multiple data to nodemcu        
       }

       if(count>20){
          count = 0;
          temperature++;
        }
        count++;
       
}


void interruptSetup()

{    

  TCCR2A = 0x02;  // This will disable the PWM on pin 3 and 11

  OCR2A = 0X7C;   // This will set the top of count to 124 for the 500Hz sample rate

  TCCR2B = 0x06;  // DON'T FORCE COMPARE, 256 PRESCALER

  TIMSK2 = 0x02;  // This will enable interrupt on match between OCR2A and Timer

  sei();          // This will make sure that the global interrupts are enable

}


ISR(TIMER2_COMPA_vect)

{ 

  cli();                                     

  analog_data = analogRead(sensor_pin);            

  samplecounter += 2;                        

  int N = samplecounter - lastBeatTime;      


  if(analog_data < thresh && N > (time_between_beats/5)*3)

    {     

      if (analog_data < trough_value)

      {                       

        trough_value = analog_data;

      }

    }


  if(analog_data > thresh && analog_data > peak_value)

    {        

      peak_value = analog_data;

    }                          



   if (N > 250)

  {                            

    if ( (analog_data > thresh) && (pulse_signal == false) && (N > (time_between_beats/5)*3) )

      {       

        pulse_signal = true;          

        digitalWrite(led_pin,HIGH);

        time_between_beats = samplecounter - lastBeatTime;

        lastBeatTime = samplecounter;     



       if(second_heartpulse)

        {                        

          second_heartpulse = false;   

          for(int i=0; i<=9; i++)    

          {            

            beat[i] = time_between_beats; //Filling the array with the heart beat values                    

          }

        }


        if(first_heartpulse)

        {                        

          first_heartpulse = false;

          second_heartpulse = true;

          sei();            

          return;           

        }  


      word runningTotal = 0;  


      for(int i=0; i<=8; i++)

        {               

          beat[i] = beat[i+1];

          runningTotal += beat[i];

        }


      beat[9] = time_between_beats;             

      runningTotal += beat[9];   

      runningTotal /= 10;        

      heart_rate = 60000/runningTotal;

    }                      

  }




  if (analog_data < thresh && pulse_signal == true)

    {  

      digitalWrite(led_pin,LOW); 

      pulse_signal = false;             

      amplitude = peak_value - trough_value;

      thresh = amplitude/2 + trough_value; 

      peak_value = thresh;           

      trough_value = thresh;

    }


  if (N > 2500)

    {                          

      thresh = 512;                     

      peak_value = 512;                 

      trough_value = 512;               

      lastBeatTime = samplecounter;     

      first_heartpulse = true;                 

      second_heartpulse = false;               

    }


  sei();                                

}
