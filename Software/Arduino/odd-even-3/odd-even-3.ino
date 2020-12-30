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

bool debug = false;

const int BAUD = 9600;
int pwmPins[4] = {3, 9, 10, 11};
const int readLEDPins[4] = {2, 4, 5, 6};
const int voltagePin = A0;
const float minThreshold = 2.6;
const float maxThreshold = 4.5;
const float nearly0 = 0.0000001;
long seconds = 0;
int pwms[4] = {0, 0, 0, 0};
int previouspwms[4];
float previousLoss;
bool inputs[4] = {false, false, false, false};
bool teaching = false;
bool exploring = false;
float threshold = 4.0;
float previousThreshold;
float learningRate = 0.2;
float thresholdLearningRate = 0.2;
long settle = 300;
long lastTime;
int bestPWMs[4];
float bestError = 3.4028235E+38;
int sample;
int totalSamples;


void SetPWMs()
{
  for(int synapse = 0; synapse < 4; synapse++)
  {
    analogWrite(pwmPins[synapse], pwms[synapse]);
  }
  delay(settle);
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
  SetLEDs(true);
  int v = analogRead(voltagePin);
  SetLEDs(false); 
  return (float)v*5.0/1024.0;
}

// Set an input pattern: 0 - 15

void SetNumber(int n)
{
  for(int synapse = 0; synapse < 4; synapse++)
  {
     inputs[synapse] = (n >> synapse) & 1;
  }
}

// Compute the mean of the sum of squared errors

float LossFunction()
{
  float wrong = 0.0;
  bool error;
  for(int n = 0; n < 16; n++)
  {
    SetNumber(n);
    float v = ReadValue();
    error = false;
    if(n & 1)
    {
      if(v > threshold)
        error = true;
    } else
    {
       if(v < threshold)
        error = true;     
    }
    float d = threshold - v;
    if(error)
    {
      wrong += d*d;
    } 
  }

  return wrong;
}

void PrintAll()
{
  for(int n = 0; n < 16; n++)
  {
    SetNumber(n);
    Serial.print(n);
    float v = ReadValue();
    if(v > threshold)
      Serial.print(" is even. dv = ");
    else
      Serial.print(" is odd. dv = ");
    Serial.println(v - threshold); 
  }
  Serial.print("Loss: ");
  Serial.println(LossFunction());
}



// Allow the user to see if it works.

void TestNumber()
{

  if(Serial.available() <= 0)
    return;
    

  int n = Serial.parseInt();

  if(n < 0)
  {
    Serial.println("Teaching stopped.");
    teaching = false;
    return;
  }

  if(n > 16)
  {
    PrintAll();
    return;
  }
  
  SetNumber(n);
  Serial.print(n);
  float v = ReadValue();
  if(v > threshold)
    Serial.print(" is even. dv = ");
  else
    Serial.print(" is odd. dv = ");
  Serial.println(v - threshold); 

  while(Serial.available()) Serial.read();
}

void Report(float loss)
{
  Serial.print("Loss: ");
  Serial.print(loss, 4);
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
  SetPWMs();
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

void Teach()
{
  if(!teaching)
    return;
    
  long t = millis();
  if(t - lastTime < settle)
    return;
  lastTime = t;
  
  int pwmAdjustments[4];
  
  float loss = LossFunction();
      
  float derivative;
  float deltaLoss = loss - previousLoss;
  if(debug)
  {
    Serial.print("Delta loss: ");
    Serial.print(deltaLoss);
    Serial.print(", delta PWM|derivative: ");
  }

  for(int synapse = 0; synapse < 4; synapse++)
  {
    int deltaPWM = pwms[synapse] - previouspwms[synapse];
    if(debug)
    {
      Serial.print(deltaPWM);
      Serial.print("|");
    }
    if(deltaPWM == 0)
    {
      deltaPWM = RandomPM1(); // random +/-1: avoid division by 0
    }
    derivative = deltaLoss/(float)(deltaPWM);
    if(debug)
    {
      Serial.print(derivative);
      Serial.print(", ");
    }
    if(fabs(derivative) < nearly0)
      pwmAdjustments[synapse] = RandomPM1(); // avoid division by 0 but still shake things up a bit
    else
      pwmAdjustments[synapse] = -round(learningRate*previousLoss/derivative); // Newton-Raphson
  }
  float deltaThreshold = threshold - previousThreshold;
  if(debug)
  {
    Serial.print("delta threshold|derivative: ");
    Serial.print(deltaThreshold);
    Serial.print("|");
  }
  if(fabs(deltaThreshold) < nearly0)
    deltaThreshold = 0.005 + copysignf(deltaThreshold, 0.01);
  derivative = deltaLoss/deltaThreshold;
  if(debug)
  {
    Serial.println(derivative);
  }
  float thresholdAdjustment;
  if(fabs(derivative) < nearly0)
    thresholdAdjustment = (float)RandomPM1()*0.01; // avoid division by 0
  else
    thresholdAdjustment = -thresholdLearningRate*previousLoss/derivative;  // Again with the Newton-Raphson
  previousLoss = loss;
  Adjust(pwmAdjustments, thresholdAdjustment);
  Report(loss);
}

void StartTeach()
{
  previousLoss = LossFunction();

  // Small purturbations to get initial derivatives.

  for(int synapse = 0; synapse < 4; synapse++)
  {
    previouspwms[synapse] = pwms[synapse];
    pwms[synapse] += RandomPM1();
  }
  
//  previousThreshold = threshold;
//  threshold += 0.02*(float)RandomPM1();
  teaching = true;
  exploring = false;
  lastTime = millis(); 
}

// Explore the space

void Explore()
{
  if(!exploring)
    return;
    
  long t = millis();
  if(t - lastTime < settle)
    return;
  lastTime = t;

  sample++;

  if(sample >= totalSamples)
  {
    Serial.print("Exploring finished.\nSetting the best result: ");
    for(int synapse = 0; synapse < 4; synapse++)
    {
      pwms[synapse] = bestPWMs[synapse];
      Serial.print(pwms[synapse]);
      Serial.print(", ");
    }
    SetPWMs();
    Serial.print(" lowest error: ");
    Serial.println(bestError);
    exploring = false;
    return;
  }

  float loss = LossFunction();
  Serial.print("Exploring step ");
  Serial.print(sample);
  Serial.print('/');
  Serial.print(totalSamples);
  Serial.print(". Current loss: ");
  Serial.print(loss);
  if(loss < bestError)
  {
    Serial.print(" is an improvement. PWMs: ");
    bestError = loss;
    for(int synapse = 0; synapse < 4; synapse++)
    {
      Serial.print(pwms[synapse]);
      if(synapse < 3)
        Serial.print(", ");
      bestPWMs[synapse] = pwms[synapse];
    }
  }
  Serial.println();
  
  for(int synapse = 0; synapse < 4; synapse++)
  {
    pwms[synapse] = random(256);
  }
  SetPWMs();
}

void StartExplore(int samples)
{
  Serial.println("Start with the current pattern (y/n)? ");
  while(Serial.available() <= 0);
  int resp = Serial.read();
  Serial.println(resp);  
  while(Serial.available()) Serial.read();
  
  if(resp == 'n')
  {
    Serial.print("Setting new random pattern: ");
    for(int synapse = 0; synapse < 4; synapse++)
    {
      pwms[synapse] = random(256);
      Serial.print(pwms[synapse]);
      if(synapse < 3)
          Serial.print(", ");
    }
    Serial.println();
  }
  SetPWMs();
  sample = 0;
  totalSamples = samples;
  exploring = true;
  teaching = false;
  lastTime = millis(); 
}

void Help()
{
  Serial.println("Commands: ");
  Serial.println(" ?: Print this list.");
  Serial.println(" E: Turn on exploring the weights space at random.");
  Serial.println(" e: Turn off exploring the weights space at random.");  
  Serial.println(" T: Turn teaching on.");
  Serial.println(" t: Turn teaching off.");
  Serial.println(" s: Setrandom seed.");
  Serial.println(" h: Set threshold.");
  Serial.println(" m: Set settling time.");      
  Serial.println(" r: Run a test.");
  Serial.println(" p: Print the current PWM pattern and threshold.");  
  Serial.println(" P: Set a PWM pattern.");
  Serial.println(" f: Set input false/true pattern.");
  Serial.println(" 0: Reset exploration.");
  Serial.println(" L: Turn the LEDs on.");
  Serial.println(" l: Turn the LEDs off.");
  Serial.println(" v: Read the voltage.");         
}

void Control()
{
  if(!Serial.available())
   return;

  int c = Serial.read();
  int n;
  switch(c)
  {
    case 'e':
      exploring = false;
      Serial.println("Not exploring.");
      break;

    case 'E':
      Serial.print("Number of points in the search space: ");
      while(Serial.available() <= 0);
      n = Serial.parseInt();
      StartExplore(n);
      Serial.println(n);  
      break;

    case 't':
      teaching = false;
      Serial.println("Not teaching.");
      break;

    case 'T':
      StartTeach();
      break;

    case 'r':
      PrintAll();
      break;

    case 's':
      Serial.print("Random seed: ");
      while(Serial.available() <= 0);
      n = Serial.parseInt();
      randomSeed(n);
      Serial.println(n);      
      break; 

    case 'm':
      Serial.print("Settling time (ms): ");
      while(Serial.available() <= 0);
      settle = Serial.parseInt();
      Serial.println(settle);      
      break;   

    case 'h':
      Serial.print("Threshold in [0.0, 5.0]: ");
      while(Serial.available() <= 0);
      threshold = Serial.parseFloat();
      Serial.println(threshold);  
      break;

    case '0':
      bestError = 3.4028235E+38;
      Serial.println("Exploration reset");  
      break;

    case 'p':
      Serial.print("PWM pattern: ");
      for(int synapse = 0; synapse < 4; synapse++)
      {
        Serial.print(pwms[synapse]);
        Serial.print(", ");
      }
      Serial.print("threashold: ");
      Serial.println(threshold);    
      break;   

    case 'P':
      Serial.print("PWM pattern (4 numbers in [0, 255]): ");
      while(Serial.available() <= 0);
      for(int synapse = 0; synapse < 4; synapse++)
      {
        pwms[synapse] = Serial.parseInt();
        Serial.print(pwms[synapse]);
        if(synapse < 3)
          Serial.print(", ");
      }
      Serial.println();
      while(Serial.available() > 0) Serial.read();
      SetPWMs();     
      break;   

   case 'f':
      Serial.print("Input pattern (4 0s or 1s): ");
      while(Serial.available() <= 0);
      for(int synapse = 0; synapse < 4; synapse++)
      {
        inputs[synapse] = Serial.parseInt();
        Serial.print(inputs[synapse]);
        if(synapse < 3)
          Serial.print(", ");
      }
      Serial.println();
      while(Serial.available() > 0) Serial.read();    
      break;    

    case 'L':
      for(int synapse = 0; synapse < 4; synapse++)
        inputs[synapse] = true;  
      SetLEDs(true);
      Serial.println("All LEDs on.");
      break;

    case 'l':  
      SetLEDs(false);
      Serial.println("All LEDs off.");
      break;

    case 'v':  
      Serial.print("Voltage: ");
      Serial.println(ReadValue());
      break;
      
    default:
    case '?':
      Help();
      break;
  }
  delay(100); // What????
  while(Serial.available()) Serial.read();
}

void setup() 
{
  for(int synapse = 0; synapse < 4; synapse++)
  {
    pinMode(pwmPins[synapse], OUTPUT);
    pinMode(readLEDPins[synapse], OUTPUT);  
  }

  pinMode(voltagePin, INPUT);
  SetPWMs();
  SetLEDs(false);

  Serial.begin(BAUD);
  Serial.println("RepRap Ltd Optical Half-Neuron Starting\n");

  // Having a timeout on serial input is very silly and annoying.
  
  Serial.setTimeout(30000);

  Help();

  // Fast PWM
  
  TCCR2B = TCCR2B & B11111000 | B00000001; // D3, D11
  TCCR1B = TCCR1B & B11111000 | B00000001; // D9, D10

  lastTime = millis();
}



void loop()
{
  Teach();
  Explore();
  Control();
  Serial.flush();
}
