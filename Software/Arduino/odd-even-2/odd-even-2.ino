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
const float minThreshold = 2.0;
const float maxThreshold = 4.7;
const float nearly0 = 0.0000001;
long seconds = 0;
int pwms[4] = {0, 0, 0, 0}; // About half way for each
int previouspwms[4];
float previousCost = 8.0; // Randomish big cost value
bool inputs[4] = {false, false, false, false};
float threshold = 4.3;
float previousThreshold;
float learningRate = 0.2;
float thresholdLearningRate = 0.2;
long settle = 120000;
long lastTime;


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

// Compute the sum of squared errors

float CostFunction()
{
  float error = 0.0;
  for(int n = 0; n < 16; n++)
  {
    SetNumber(n);
    float v = ReadValue();
    if((v < threshold && !(n & 1) ) || (v > threshold && (n & 1)))
    {
      float d = threshold - v;
      error += d*d;
    }
  }

  return error;
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

void Report(float cost)
{
  Serial.print("Cost: ");
  Serial.print(cost, 4);
  Serial.print(", PWMs: ");
  for(int synapse = 0; synapse < 4; synapse++)
  {
    Serial.print(pwms[synapse]);
    Serial.print(", "); 
  }
  Serial.print(" Threshold: ");
  Serial.println(threshold, 4);
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
  if(threshold < minThreshold)
    threshold = minThreshold;
  if(threshold > maxThreshold)
    threshold = maxThreshold;
  SetPWMs(true);
}

// + or - 1

int RandomPM1()
{
  return 1 - random(2)*2;
}

void RandomPWMs()
{
    for(int synapse = 0; synapse < 4; synapse++)
    {
      pwms[synapse] = random(70);
    }  
}

float Teach()
{
  long t = millis();
  if(t - lastTime < settle)
    return;
  lastTime = t;
  
  int pwmAdjustments[4];
  
  float cost = CostFunction();
      
  float derivative;
  float deltaCost = cost - previousCost;

  for(int synapse = 0; synapse < 4; synapse++)
  {
    int deltaPWM = pwms[synapse] - previouspwms[synapse];
    if(deltaPWM == 0)
    {
      deltaPWM = RandomPM1(); // random +/-1: avoid division by 0
    }
    derivative = deltaCost/(float)(deltaPWM);
    if(fabs(derivative) < nearly0)
      pwmAdjustments[synapse] = RandomPM1(); // avoid division by 0 but still shake things up a bit
    else
      pwmAdjustments[synapse] = -round(learningRate*previousCost/derivative); // Newton-Raphson
  }
  float deltaThreshold = threshold - previousThreshold;
  if(fabs(deltaThreshold) < nearly0)
    deltaThreshold = 0.005 + copysignf(deltaThreshold, 0.01);
  derivative = deltaCost/deltaThreshold;
  float thresholdAdjustment;
  if(fabs(derivative) < nearly0)
    thresholdAdjustment = (float)RandomPM1()*0.01; // avoid division by 0
  else
    thresholdAdjustment = -thresholdLearningRate*previousCost/derivative;  // Again with the Newton-Raphson
  previousCost = cost;
  Adjust(pwmAdjustments, thresholdAdjustment);
  Report(cost);

  return cost;
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
  Serial.println("Type an integer in [0, 15] at any time to test if it works.");
  Serial.print("Random seed: ");
  while(Serial.available() <= 0);
  int n = Serial.parseInt();
  while(Serial.available()) Serial.read();
  Serial.println(n); 
  randomSeed(n);

  
  Serial.print("4 starting PWMs (-1 to use random): ");

  while(Serial.available() <= 0);
  n = Serial.parseInt();
  while(Serial.available()) Serial.read();
  Serial.println(n); 
    
  if(n < 0)
  {
    RandomPWMs();
  } else
  {
    pwms[0] = n;
    for(int synapse = 1; synapse < 4; synapse++)
    {
      while(Serial.available() <= 0);
      pwms[synapse] = Serial.parseInt();
      while(Serial.available()) Serial.read();
    }  
  }

  SetPWMs(true);  

  Serial.print("Initial threshold: ");
  while(Serial.available() <= 0);
  threshold = Serial.parseFloat();
  while(Serial.available()) Serial.read();
  Serial.println(threshold);

  Serial.print("Settling time (ms): ");
  while(Serial.available() <= 0);
  settle = Serial.parseInt();
  while(Serial.available()) Serial.read();
  Serial.println(settle); 


  // Having a timeout on serial input is very silly and annoying.
  
  Serial.setTimeout(30000);
  for(int synapse = 0; synapse < 4; synapse++)
    previouspwms[synapse] = pwms[synapse];
  Report(-1.0);
  delay(settle);
  previousCost = CostFunction();

  // Small purturbations to get initial derivatives.

  for(int synapse = 0; synapse < 4; synapse++)
  {
    previouspwms[synapse] = pwms[synapse];
    pwms[synapse] += RandomPM1();
  }
  
  previousThreshold = threshold;
  threshold += 0.02*(float)RandomPM1();

  Report(previousCost);
  lastTime = millis();
}



void loop()
{
  Teach();

  TestNumber();
}