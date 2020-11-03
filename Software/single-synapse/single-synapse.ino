/*
 * RepRap Ltd Open-source Optical Synapse Test Program
 * 
 * https://reprapltd.com
 * 
 * Device: Arduino Uno
 * 
 * Adrian Bowyer
 * 21 October 2020
 * 
 * Licence: GPL
 * 
 * See: https://github.com/RepRapLtd/OpticalNeuralNet
 * 
 */

const int BAUD = 9600;
const int uVLEDPin = 3;
const int ledMonitorPin = 4;
const int readLEDPin = 5;
const int voltagePin = A0;
long seconds = 0;
int pwm = 0;

void SetValue(int v)
{
  if(v <= 0) v = 0;
  if(v > 255) v = 255;
  analogWrite(uVLEDPin, v);
  pwm = v;
}

float ReadValue(bool dark)
{
  //while(!digitalRead(ledMonitorPin));
  digitalWrite(uVLEDPin, 0);
  if(!dark)
    digitalWrite(readLEDPin, 1);
  int v = analogRead(voltagePin);
  digitalWrite(readLEDPin, 0);
  analogWrite(uVLEDPin, pwm);
  return (float)v*5.0/1024.0;
}


// Allow the user to change values.

void Change()
{

  if(Serial.available() <= 0)
    return;
    
  int v = Serial.parseInt();
  SetValue(v);
  while(Serial.available()) Serial.read();
}

void setup() 
{
  pinMode(uVLEDPin, OUTPUT);
  pinMode(readLEDPin, OUTPUT);  
  pinMode(ledMonitorPin, INPUT);
  pinMode(voltagePin, INPUT);
  SetValue(0);
  digitalWrite(readLEDPin, 0);

  Serial.begin(BAUD);
  Serial.println("RepRap Ltd Optical Synapse Starting");

  // Having a timeout on serial input is very silly and annoying.
  
  Serial.setTimeout(30000);
}

void loop()
{
  Change();
  Serial.print(seconds);
  Serial.print(", ");
  Serial.print(pwm);
  Serial.print(", ");
  Serial.print(ReadValue(true));
  Serial.print(", ");
  Serial.println(ReadValue(false)); 
  delay(1000);
  seconds++; 
}
