
const int Output_Pin = 2;

volatile int  Pulse_Count;
unsigned int  Liter_per_hour;
unsigned long Current_Time, Loop_Time;

void setup()
{ 
   pinMode(Output_Pin, INPUT);
   Serial.begin(9600);
   attachInterrupt(0, Detect_Rising_Edge, FALLING);
                                     
   Current_Time = millis();
   Loop_Time = Current_Time;
   Serial.println("Water Flow Meter");
   Serial.println("Microcontrollers Lab");
} 

void loop ()    
{
   Current_Time = millis();
   if(Current_Time >= (Loop_Time + 1000))
   {
      Loop_Time = Current_Time;
      Liter_per_hour = (Pulse_Count * 60 / 7.5);
      
       if(Pulse_Count != 0){
      
      Serial.print(Liter_per_hour);
      Serial.println(" L/M");
      Pulse_Count = 0;
      Serial.print(Liter_per_hour, DEC);
      Serial.println(" Liter/hour");
       }
       else {
      Serial.println(" flow rate = 0 ");
      Serial.println("Rate: ");
      Serial.print( Liter_per_hour );
      Serial.print(" L/M");
    }
   }
   
}
void Detect_Rising_Edge ()
{ 
   Pulse_Count++;
} 
