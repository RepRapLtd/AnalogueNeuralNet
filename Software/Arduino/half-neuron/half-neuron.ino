/*
 * RepRap Ltd Open-source Optical Neuron Test Program
 * 
 * https://reprapltd.com
 * 
 * Device: Arduino Uno
 * 
 * Adrian Bowyer
 * 24 Novem 2020
 * 
 * Licence: GPL
 * 
 * See: https://github.com/RepRapLtd/OpticalNeuralNet
 * 
 */

const int BAUD = 9600;
int uVLEDPins[4] = {3, 5, 6, 9};
const int readLEDPins[4] = {2, 4, 7, 8};
const int voltagePin = A0;
long seconds = 0;
int pwms[4] = {0, 0, 0, 0};
bool inputs[4] = {false, false, false, false};

void SetPWM(int v, int pwm)
{
  if(v <= 0) v = 0;
  if(v > 255) v = 255;
  analogWrite(uVLEDPins[pwm], v);
  pwms[pwm] = v;
}

void SetPWMs(bool light)
{
  for(int pwm = 0; pwm < 4; pwm++)
  {
    if(light)
    {
      analogWrite(uVLEDPins[pwm], pwms[pwm]);
    } else
    {

      digitalWrite(uVLEDPins[pwm], 0);
    }
  }
}

void SetLEDs(bool light)
{
  for(int led = 0; led < 4; led++)
  {
    if(light)
    {
      if(inputs[led])
       digitalWrite(readLEDPins[led], 1);
      else
       digitalWrite(readLEDPins[led], 0);
    } else
    {
      digitalWrite(readLEDPins[led], 0);
    }
  }
}

float ReadValue()
{
  SetPWMs(false);
  SetLEDs(true);
  int v = analogRead(voltagePin);
  SetLEDs(false);
  SetPWMs(true);  
  return (float)v*5.0/1024.0;
}

void PrintInputScan()
{
  for(int i = 0; i < 16; i++)
  {
    for(int j = 0; j < 4; j++)
     inputs[j] = (i >> j) & 1;
    Serial.print(i);
    Serial.print(", ");
    Serial.print(ReadValue());
    Serial.print(" | ");
  }
  Serial.println();
}


// Allow the user to change values.
/*
void Change()
{

  if(Serial.available() <= 0)
    return;
    
  int c = Serial.Read();
  switch(c)
  {
    Serial.parseInt();
  }
  if(v >= 0)
   SetValue(v);
  else
  {
    if(v == -1)
    {
      digitalWrite(readLEDPin, 1);
      delay(5000);
    }else
      digitalWrite(readLEDPin, 0);
  }
  while(Serial.available()) Serial.read();
}
*/
void setup() 
{
  for(int synapse = 0; synapse < 4; synapse++)
  {
    pinMode(uVLEDPins[synapse], OUTPUT);
    pinMode(readLEDPins[synapse], OUTPUT);  
  }

  pinMode(voltagePin, INPUT);
  SetPWMs(false);
  SetLEDs(false);

  Serial.begin(BAUD);
  Serial.println("RepRap Ltd Optical HAlf-Neuron Starting");

  // Having a timeout on serial input is very silly and annoying.
  
  Serial.setTimeout(30000);
}

void loop()
{
  PrintInputScan();
  while(true); 
}
