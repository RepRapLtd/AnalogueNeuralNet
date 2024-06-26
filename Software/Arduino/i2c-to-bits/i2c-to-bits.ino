// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>

int pin0 = 2;
int i2cAddress = 4;
int volts = A0;

void setup()
{
  Wire.begin(i2cAddress);                // join i2c bus with address 
  Wire.onReceive(receiveEvent); // register event
  for (int i = 0; i < 4; i++)
  {
    pinMode(pin0 + i, OUTPUT);
  }
  Serial.begin(115200);           // start serial for output
  Serial.print("\nI2C to pins");
}

void loop()
{
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  char c; // receive byte as a character
  bool print;
  while(Wire.available() > 0) // loop through all (should only be one)
  {
    c = Wire.read(); // receive byte as a character
    print = c & 0x10;
    if (c & 0x20)
    {
      float v = (float)analogRead(volts)*5.0/1024.0;
      Serial.println(v, 4);
    } else
    {
      if (print) Serial.print("\nPins set to: ");
      for (int i = 0; i < 4; i++)
      {
        if(c & 1)
        {
          digitalWrite(pin0 + i, 1);
          if (print) Serial.print("1");
        } else
        {
          digitalWrite(pin0 + i, 0);
          if (print) Serial.print("0");
        }
        c = c >> 1;
      }
    }
  }
  
  if (print) Serial.println();         
}
