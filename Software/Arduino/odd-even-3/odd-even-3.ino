/*
 * RepRap Ltd Open-source Optical Neuron Test Program
 * 
 * Discriminate odd and even numbers
 * 
 * https://reprapltd.com
 * 
 * Device: Arduino Mega
 * 
 * Adrian Bowyer
 * 26 Novem 2020
 * 
 * Licence: GPL
 * 
 * See: https://github.com/RepRapLtd/OpticalNeuralNet
 * 
 */

#define synapseCount 8
#define inputCount (synapseCount/2)
#define inputPatterns (1 << inputCount) 

bool debug = false;

const int BAUD = 9600;
int pwmPins[synapseCount] = {13, 11, 12, 10, 7, 6, 5, 4};
const int readLEDPins[synapseCount] = {46, 48, 50, 52, 38, 40, 42, 44};
const int voltagePin0 = A0;
const int voltagePin1 = A1;
const float minThreshold = 2.6;
const float maxThreshold = 4.5;
const float nearly0 = 0.0000001;
long seconds = 0;
int pwms[synapseCount] = {0, 0, 0, 0, 0, 0, 0, 0};
int previouspwms[synapseCount];
int pwmGradients[synapseCount] = {0, 0, 0, 0, 0, 0, 0, 0};
float previousLoss;
bool inputs[inputCount] = {false, false, false, false};
bool teaching = false;
bool exploring = false;
float threshold = 0.0;
float previousThreshold;
float learningRate = 0.2;
float thresholdLearningRate = 0.2;
long settle = 300;
long lastTime;
int bestPWMs[synapseCount];
float bestError = 3.4028235E+38;
int sample;
int totalSamples;
float lastv0, lastv1;
bool sawtooth = false;


// Fast PWM frequency of 31.37255 KHz...

void FastPWM()
{
  /*
  // Uno
  
  TCCR2B = TCCR2B & B11111000 | B00000001; // D3, D11
  TCCR1B = TCCR1B & B11111000 | B00000001; // D9, D10
  
  */

  // MEGA

  TCCR0B = TCCR0B & B11111000 | B00000001; // D4 D13 (60 KHz)
  TCCR1B = TCCR1B & B11111000 | B00000001; // D11 D12
  TCCR2B = TCCR2B & B11111000 | B00000001; // D9 D10
  TCCR4B = TCCR4B & B11111000 | B00000001; // D2 D3 D5
  TCCR4B = TCCR4B & B11111000 | B00000001; // D6 D7 D8
  TCCR5B = TCCR5B & B11111000 | B00000001; // D44 D45 D46
}

void SetPWMs()
{
  for(int synapse = 0; synapse < synapseCount; synapse++)
  {
    analogWrite(pwmPins[synapse], pwms[synapse]);
  }
  delay(settle);
}

void SetLEDs(bool light)
{
  for(int synapse = 0; synapse < synapseCount; synapse++)
  {
    if(light)
    {
      if(inputs[synapse%inputCount])
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
  int v0 = analogRead(voltagePin0);
  int v1 = analogRead(voltagePin1);
  SetLEDs(false);
  lastv0 = (float)v0*5.0/1024.0;
  lastv1 = (float)v1*5.0/1024.0;
  return lastv0 - lastv1;
}

// Set an input pattern

void SetNumber(int n)
{
  for(int input = 0; input < inputCount; input++)
  {
     inputs[input] = (n >> input) & 1;
  }
}

// Compute the mean of the sum of squared errors
// Optionally print the patterns and voltages

float LossFunction(bool output)
{
  float wrong = 0.0;
  bool error;
  
  for(int n = 0; n < inputPatterns; n++)
  {
    SetNumber(n);
    if(output) Serial.print(n);
    float v = ReadValue();
    error = false;
    float dv = threshold - v;
    
    if(n <= 7)
    {
      if(dv < 0.0)
        error = true;
    } else
    {
       if(dv > 0.0)
        error = true;     
    }
    
    if(error)
    {
      wrong += 1; //dv*dv;
    }
    
    if(output)
    {
      if(dv >= 0.0)
        Serial.print(" is <= 7.");
      else
        Serial.print(" is > 7.");
      Serial.print(" v0 = ");
      Serial.print(lastv0);
      Serial.print(", v1 = ");
      Serial.print(lastv1);
      Serial.print(", dv = ");      
      Serial.println(dv); 
    }
  }
  
  if(output)
  {
    Serial.print("Loss: ");
    Serial.println(wrong);
  }
  
  return wrong;
}

/*
void PrintAll()
{
  for(int n = 0; n < inputPatterns; n++)
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
*/


// Allow the user to see if it works.
/*
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

  if(n > inputPatterns)
  {
    LossFunction(true);
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
*/

void Report(float loss)
{
  Serial.print("Loss: ");
  Serial.print(loss, 4);
  Serial.print(", PWMs: ");
  for(int synapse = 0; synapse < synapseCount; synapse++)
  {
    Serial.print(pwms[synapse]);
    Serial.print(", "); 
  }
  Serial.print(" Threshold: ");
  Serial.println(threshold, 4);
}


void Adjust(int pwmAdjustments[], float thresholdAdjustment)
{
  for(int synapse = 0; synapse < synapseCount; synapse++)
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
    for(int synapse = 0; synapse < synapseCount; synapse++)
    {
      pwms[synapse] = random(70);
    }  
}

void Teach()
{
  if(!teaching)
    return;
    
//  long t = millis();
//  if(t - lastTime < settle)
//    return;
//  lastTime = t;
  
  int pwmAdjustments[synapseCount];
  
  float loss = LossFunction(false);
      
  float derivative;
  float deltaLoss = loss - previousLoss;
  if(debug)
  {
    Serial.print("Delta loss: ");
    Serial.print(deltaLoss);
    Serial.print(", delta PWM|derivative: ");
  }

  for(int synapse = 0; synapse < synapseCount; synapse++)
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
  previousLoss = LossFunction(false);

  // Small purturbations to get initial derivatives.

  for(int synapse = 0; synapse < synapseCount; synapse++)
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

void ReportPWMsAndSetBest()
{
    for(int synapse = 0; synapse < synapseCount; synapse++)
    {
      pwms[synapse] = bestPWMs[synapse];
      Serial.print(pwms[synapse]);
      Serial.print(", ");
    }
    SetPWMs();
    Serial.print(" lowest error: ");
    Serial.println(bestError);
    Serial.print("Gradients: ");
    for(int synapse = 0; synapse < synapseCount; synapse++)
    {
      Serial.print(pwmGradients[synapse]);
      if(synapse < synapseCount - 1)
        Serial.print(", ");
    }
    Serial.println();  
}

// Explore the space

void Explore()
{
  if(!exploring)
    return;
    
//  long t = millis();
//  if(t - lastTime < settle)
//    return;
//  lastTime = t;

  sample++;

  if(sample >= totalSamples)
  {
    Serial.print("Exploring finished.\nSetting the best result: ");
    ReportPWMsAndSetBest();
    exploring = false;
    return;
  }

  float loss = LossFunction(false);
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
    for(int synapse = 0; synapse < synapseCount; synapse++)
    {
      Serial.print(pwms[synapse]);
      if(synapse < synapseCount-1)
        Serial.print(", ");
      pwmGradients[synapse] = pwms[synapse] - bestPWMs[synapse];
      bestPWMs[synapse] = pwms[synapse];
    }
    Serial.println();
    if(loss == 0.0)
    {
      Serial.print("\nExploring stopped at zero loss. PWMs: ");
      ReportPWMsAndSetBest();
      exploring = false;
      return;
    }
  }
  Serial.println();
  
  for(int synapse = 0; synapse < synapseCount; synapse++)
  {
    pwms[synapse] = random(256);
  }
  SetPWMs();
}

void StartExplore(int samples)
{
/*  Serial.println("Start with the current pattern (y/n)? ");
  while(Serial.available() <= 0);
  int resp = Serial.read();
  Serial.println(resp);  
  while(Serial.available()) Serial.read();
  
  if(resp == 'n')
  {*/
    Serial.print("Setting new random pattern: ");
    for(int synapse = 0; synapse < synapseCount; synapse++)
    {
      pwms[synapse] = random(256);
      Serial.print(pwms[synapse]);
      if(synapse < synapseCount-1)
          Serial.print(", ");
    }
    Serial.println();
 // }
  SetPWMs();
  sample = 0;
  totalSamples = samples;
  exploring = true;
  teaching = false;
  lastTime = millis(); 
}

void Calibrate()
{
  Serial.println("\nCalibration: ");
  for(int pwm = 0; pwm < 256; pwm += 8)
  {
    for(int synapse = 0; synapse < synapseCount; synapse++)
    {
      pwms[synapse] = pwm;
    }
    SetPWMs();
    Serial.print(pwm);
    Serial.print(' ');
    for(int input = 0; input < inputCount; input++)
    {
      for(int i = 0; i < inputCount; i++)
      {
        inputs[i] = 0;
      }
      inputs[input] = 1;
      float v = ReadValue();
      Serial.print(lastv0);
      Serial.print(' ');
      Serial.print(lastv1);
      Serial.print(' ');
    }
    Serial.println();
  }
  Serial.println();
}

void Saw()
{
  if(!sawtooth)
    return;
    
  for(int input = 0; input < inputCount; input++)
  {
        inputs[input] = 1;
  }
  
  SetLEDs(true);
  
  for(int pwm = 0; pwm < 256; pwm += 8)
  {
    for(int synapse = 0; synapse < synapseCount; synapse++)
    {
      pwms[synapse] = pwm;
    }
    SetPWMs();
  }
}

void Help()
{
  Serial.println("Commands: ");
  Serial.println(" ?: Print this list.");
  Serial.println(" E: Turn on exploring the weights space at random.");
  Serial.println(" e: Turn off exploring the weights space at random.");  
  Serial.println(" T: Turn teaching on.");
  Serial.println(" t: Turn teaching off.");
  Serial.println(" s: Set random seed.");
  Serial.println(" h: Set threshold.");
  Serial.println(" m: Set settling time.");
  Serial.println(" c: Calibrate.");        
  Serial.println(" r: Run a test.");
  Serial.println(" p: Print the current PWM pattern and threshold.");  
  Serial.println(" P: Set a PWM pattern.");
  Serial.println(" f: Set input false/true pattern.");
  Serial.println(" 0: Reset exploration.");
  Serial.println(" L: Turn the LEDs on.");
  Serial.println(" l: Turn the LEDs off.");
  Serial.println(" v: Read the voltage.");
  Serial.println(" /: Continuous sawtooth.");
  Serial.println(" -: Sawtooth off.");           
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
      LossFunction(true);
      break;

    case 's':
      Serial.print("Random seed: ");
      while(Serial.available() <= 0);
      n = Serial.parseInt();
      randomSeed(n);
      Serial.println(n);      
      break;

    case 'c':
      Calibrate();
      break;

    case 'm':
      Serial.print("Settling time (ms): ");
      while(Serial.available() <= 0);
      settle = Serial.parseInt();
      Serial.println(settle);      
      break;   

    case 'h':
      Serial.print("Threshold in [-5.0, 5.0]: ");
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
      for(int synapse = 0; synapse < synapseCount; synapse++)
      {
        Serial.print(pwms[synapse]);
        Serial.print(", ");
      }
      Serial.print("threashold: ");
      Serial.println(threshold);    
      break;   

    case 'P':
      Serial.print("PWM pattern (synapseCount numbers in [0, 255]): ");
      while(Serial.available() <= 0);
      for(int synapse = 0; synapse < synapseCount; synapse++)
      {
        pwms[synapse] = Serial.parseInt();
        Serial.print(pwms[synapse]);
        if(synapse < synapseCount-1)
          Serial.print(", ");
      }
      Serial.println();
      while(Serial.available() > 0) Serial.read();
      SetPWMs();     
      break;   

   case 'f':
      Serial.print("Input pattern (");
      Serial.print(inputCount);
      Serial.print(" 0s or 1s): ");
      while(Serial.available() <= 0);
      for(int input = 0; input < inputCount; input++)
      {
        inputs[input] = Serial.parseInt();
        Serial.print(inputs[input]);
        if(input < inputCount-1)
          Serial.print(", ");
      }
      Serial.println();
      SetLEDs(true);
      while(Serial.available() > 0) Serial.read();    
      break;    

    case 'L':
      for(int input = 0; input < inputCount; input++)
        inputs[input] = true;  
      SetLEDs(true);
      Serial.println("All LEDs on.");
      break;

    case 'l':  
      SetLEDs(false);
      Serial.println("All LEDs off.");
      break;

    case '/':
      Serial.println("Sawtooth on");  
      sawtooth = true;
      break;

    case '-':
      Serial.println("Sawtooth off");  
      sawtooth = false;
      break;

    case 'v':  
      Serial.print("Voltage: ");
      Serial.print(ReadValue());
      Serial.print(" (v0 = ");
      Serial.print(lastv0);
      Serial.print(", v1 = ");
      Serial.print(lastv1);
      Serial.print(", threshold = ");
      Serial.print(threshold);
      Serial.println(")");
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
  for(int synapse = 0; synapse < synapseCount; synapse++)
  {
    pinMode(pwmPins[synapse], OUTPUT);
    pinMode(readLEDPins[synapse], OUTPUT);  
  }

  pinMode(voltagePin0, INPUT);
  pinMode(voltagePin1, INPUT);  
  
  SetPWMs();
  SetLEDs(false);

  Serial.begin(BAUD);
  Serial.println("RepRap Ltd Optical Neuron Starting\n");

  // Having a timeout on serial input is very silly and annoying.
  
  Serial.setTimeout(30000);

  Help();

  FastPWM();

  lastTime = millis();
}



void loop()
{
  Teach();
  Explore();
  Saw();
  Control();
  Serial.flush();
}
