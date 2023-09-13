

#include "SparkFun_Tlc5940.h"

#define PCOUNT 4
#define DCOUNT 2

//int pins[PCOUNT] = {2, 3, 11, 12};
int ds[DCOUNT] = {6, 7};

void setup() 
{
 
  Tlc.init(4095);
  Serial.begin(9600);
  //Serial.setTimeout(100000);
/*
  for(int p = 0; p < PCOUNT; p++)
  {
    pinMode(pins[p], OUTPUT);
    analogWrite(pins[p], 0);
  }
*/
  for(int d = 0; d < DCOUNT; d++)
  {
    pinMode(ds[d], OUTPUT);
    digitalWrite(ds[d], 0);
  }

  Serial.println("Starting..."); 
}

void commands()
{
  Serial.println("Commands:");
  Serial.println(" p n v - set pwm on pin p to v");
  Serial.println(" 0 - set all pwms to 0 and all dendrites off");
  Serial.println(" 1 - set all dendrites on");
  Serial.println(" d n v - set dendrite n to v");
}

void loop()
{
  char c;
  int p, d, v;
  if (Serial.available() > 0) 
  {
    // read the incoming byte:
    c = (char)Serial.read();
    switch(c)
    {
      case 'p': 
        p = Serial.parseInt();
        v = 4095 - Serial.parseInt();
        Serial.print("Setting pin ");
        Serial.print(p);
        Serial.print(" to ");
        Serial.println(v);
        Tlc.set(p, v);
        Tlc.update();
        break;

      case '0':
        for (int p = 0; p < PCOUNT; p++)
        {
          Tlc.set(p, 4095);
        }
        Serial.println("All PWMs off");
        Tlc.update();
        break;

      case '1':
        for(int d = 0; d < DCOUNT; d++)
        {
          digitalWrite(ds[d], 0);
        }
        Serial.println("Dendrites set to 0 (on)");
        break;

      case 'd':
        d = Serial.parseInt();
        v = Serial.parseInt();
        Serial.print("Setting dendrite ");
        Serial.print(ds[d]);
        Serial.print(" to ");
        Serial.println(v);
        digitalWrite(ds[d], v);
        break;

      case '\n':
      case '\r':
      case ' ':
        break;

      default:
        commands();                
    }
  }
}
