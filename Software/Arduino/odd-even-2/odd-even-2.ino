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
float previousCost = 64.0; // What you'd expect from half right at random
bool inputs[4] = {false, false, false, false};
float threshold = 4.3;
float previousThreshold;
float learningRate = 1.0;
float thresholdLearningRate = 0.3;

void SetPWM(int v, int pwm)
{
  if(v <= 0) v = 0;
  if(v > 255) v = 255;
  analogWrite(uVLEDPins[pwm], v);
  pwms[pwm] = v;
}

void SetPWMs(bool light)
{
  for(int synapse = 0; synapse < 4; synapse++)
  {
    if(light)
    {
      analogWrite(uVLEDPins[synapse], pwms[synapse]);
    } else
    {
      digitalWrite(uVLEDPins[synapse], 0);
    }
  }
}

void SetLEDs(bool light)
{
  for(int synapse = 0; synapse < 4; synapse++)
  {
    if(light)
    {
      if(inputs[synapse])
       digitalWrite(readLEDPins[synapse], 1);
      else
       digitalWrite(readLEDPins[synapse], 0);
    } else
    {
      digitalWrite(readLEDPins[synapse], 0);
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
  for(int synapse = 0; synapse < 4; synapse++)
  {
    Serial.print(pwms[synapse]);
    Serial.print(", ");
  }
  for(int n = 0; n < 16; n++)
  {
    for(int synapse = 0; synapse < 4; synapse++)
    {
     inputs[synapse] = (n >> synapse) & 1;
    }
    Serial.print(ReadValue());
    if(n != 15)
     Serial.print(", ");
  }
  Serial.println();
}

// Set an input pattern: 0 - 15

void SetNumber(int n)
{
  for(int synapse = 0; synapse < 4; synapse++)
  {
     inputs[synapse] = (n >> synapse) & 1;
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
  float cost = (float)(wrong*wrong); // Nice tangent at 0
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

bool Report(char* title, float cost)
{
  Serial.println(title);
  Serial.print(' ');
  Serial.print("Cost: ");
  Serial.print(cost);
  Serial.print(", PWMs: ");
  for(int synapse = 0; synapse < 4; synapse++)
  {
    Serial.print(pwms[synapse]);
    Serial.print(", "); 
  }
  Serial.print(" Threshold: ");
  Serial.println(threshold);
  return cost < 1.1;
}


void Adjust(int pwmAdjustments[], float thresholdAdjustment)
{
  for(int synapse = 0; synapse < 4; synapse++)
  {
      previouspwms[synapse] = pwms[synapse];
      pwms[synapse] += pwmAdjustments[synapse];
      if(pwms[synapse] < 0)
       pwms[synapse] = 0;
      if(pwms[synapse] > 255)
       pwms[synapse] = 255;
  }
  previousThreshold = threshold;
  threshold += thresholdAdjustment;
  if(threshold < 2.0)
    threshold = 2.0;
  if(threshold > 4.7)
    threshold = 4.7;
  SetPWMs(true);
}

int RandomPM1()
{
  return 1 - (2 - random(2)*2);
}

void Teach()
{
  int pwmAdjustments[4];
  
  delay(120000);
  float cost = CostFunction();
  if(Report("Before - ", cost))
  {
    Serial.println("Optimised.");
    return;
  }
      
  float derivative;
  for(int synapse = 0; synapse < 4; synapse++)
  {
    int pd = pwms[synapse] - previouspwms[synapse];
    if(pd == 0)
      pd = RandomPM1(); // random +/-1: avoid division by 0
    derivative = (cost - previousCost)/(float)(pd);
    if(derivative == 0.0)
     derivative = 1.0 - (float)(2 - (micros() & 1)); // avoid division by 0
    pwmAdjustments[synapse] = -round(learningRate*previousCost/derivative); // Newton-Raphson
  }
  derivative = (cost - previousCost)/(threshold - previousThreshold);
  if(derivative == 0.0)
   derivative = (float)RandomPM1(); // avoid division by 0
  float thresholdAdjustment = -thresholdLearningRate*previousCost/derivative;
  previousCost = cost;
  Adjust(pwmAdjustments, thresholdAdjustment);
  Report("After - ", cost);
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

  randomSeed(951);

  Serial.begin(BAUD);
  Serial.println("RepRap Ltd Optical Half-Neuron Starting");

  // Random pattern to start
  
  for(int synapse = 0; synapse < 4; synapse++)
  {
    pwms[synapse] = random(70);
  }

  SetPWMs(true);   

  // Having a timeout on serial input is very silly and annoying.
  
  Serial.setTimeout(30000);

  for(int synapse = 0; synapse < 4; synapse++)
    previouspwms[synapse] = pwms[synapse];
  Report("Initial - ", -1.0);
  delay(120000);
  previousCost = CostFunction();

  // Small purturbations to get initial derivatives.

  for(int synapse = 0; synapse < 4; synapse++)
  {
    previouspwms[synapse] = pwms[synapse];
    pwms[synapse] += RandomPM1();
  }
  previousThreshold = threshold;
  threshold += 0.05*(float)RandomPM1();

  Report("2 minutes - ", previousCost);
}

void loop()
{
  Teach();
}
