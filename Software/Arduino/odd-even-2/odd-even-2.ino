/*
 * RepRap Ltd Open-source Optical Neuron Test Program
 * 
 * Discriminate odd and even numbers
 * 
 * https://reprapltd.com
 * 
 * Device: Arduino Uno
 * 
 * Adrian Bowyer
 * 26 Novem 2020
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
int pwms[4] = {20, 20, 25, 23}; // About half way for each
int previouspwms[4];
int previousCost = 0.5;
bool inputs[4] = {false, false, false, false};
float threshold = 4.3;
float derivatives[4] = {0.0, 0.0, 0.0, 0.0};

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
  for(int i = 0; i < 4; i++)
  {
    Serial.print(pwms[i]);
    Serial.print(", ");
  }
  for(int i = 0; i < 16; i++)
  {
    for(int j = 0; j < 4; j++)
    {
     inputs[j] = (i >> j) & 1;
    }
    Serial.print(ReadValue());
    Serial.print(", ");
  }
  Serial.println();
}

// Set an input pattern: 0 - 15

void SetNumber(int n)
{
  for(int j = 0; j < 4; j++)
  {
     inputs[j] = (n >> j) & 1;
  }
}

// Compute the sum of errors
// Note errors can only be 1 or 0, so
// the sum of their squares is equal to their sum.

float CostFunction()
{
  int wrong = 0;
  for(int n = 0; n < 16; n++)
  {
    SetNumber(n);
    float v = ReadValue();
    if(v < threshold && !(n & 1))
      wrong++;
    if(v > threshold && (n & 1))
      wrong++;
  }
  float cost = (float)wrong/16.0;
  return cost;
}


// Allow the user to see if it works.

void TestNumber()
{

  if(Serial.available() <= 0)
    return;
    

  int n = Serial.parseInt();
  SetNumber(n);
  Serial.print(n);
  float v = ReadValue();
  if(v > threshold)
    Serial.println(" is even.");
  else
    Serial.println(" is odd."); 

  while(Serial.available()) Serial.read();
}

void Teach()
{
  delay(120000);
  float cost = CostFunction();
  Serial.print(cost);
  Serial.print(", ");
  for(int i = 0; i < 4; i++)
  {
      Serial.print(pwms[i]);
      Serial.print(", ");
  }
  for(int i = 0; i < 4; i++)
  {
    if(pwms[i] != previouspwms[i])
      derivatives[i] = (cost - previousCost)/(float)(pwms[i] - previouspwms[i]);
    else
      derivatives[i] = (cost - previousCost); //????
    previouspwms[i] = pwms[i];
    int adjustment = -(int)(0.5 + 1.0/derivatives[i]);
    Serial.print(adjustment);
    if(i != 3)
     Serial.print(", ");
    pwms[i] = pwms[i] + adjustment;
    if(pwms[i] < 0)
      pwms[i] = 0;
    if(pwms[i] > 255)
      pwms[i] = 255;
  }
  previousCost = cost;
  Serial.println();
  SetPWMs(true);  
}

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
  Serial.println("RepRap Ltd Optical Half-Neuron Starting");

  SetPWMs(true);   

  // Having a timeout on serial input is very silly and annoying.
  
  Serial.setTimeout(30000);

  for(int i = 0; i < 4; i++)
    previouspwms[i] = pwms[i];
  delay(120000);
  Serial.println("2 mins");
  previousCost = CostFunction();
  for(int i = 0; i < 4; i++)
  {
    if(i & 1)
      pwms[i]++;
    else
      pwms[i]--;
  }
}

void loop()
{
  Teach();
}
