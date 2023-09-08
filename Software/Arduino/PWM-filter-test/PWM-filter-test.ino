

#include "TimerOne.h"
#include "TimerThree.h"
#include "PWM.h"

#define PCOUNT 4

int pins[PCOUNT] = {2, 3, 11, 12};
void setup() 
{
  InitTimersSafe(); 
  Timer1.initialize(500); //uS
  Timer3.initialize(500);
 
  Serial.begin(9600);
  Serial.setTimeout(100000);

  bool success = true;
  for(int p = 0; p < PCOUNT; p++)
  {
    pinMode(pins[p], OUTPUT);
    analogWrite(pins[p], 0);
  }
  
  if(success) 
    Serial.println("Set frequency worked :)");  
  else
    Serial.println("Set frequency failed :("); 
}

void loop()
{
  int p = Serial.parseInt();
  int v = Serial.parseInt();
  Serial.print("Setting pin ");
  Serial.print(pins[p]);
  Serial.print(" to ");
  Serial.println(v);
  analogWrite(pins[p], v);
}
