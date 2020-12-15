

const int pwmPin = 3;
const int ledPin = 2;
const int vPin = A0;

const int writeStabilisation = 100;
const int readStabilisation = 1;


//bool saw = false;
int led = 0;

void setup() {
  TCCR2B = TCCR2B & B11111000 | B00000001; // for PWM frequency of 31372.55 Hz
  Serial.begin(9600);
  Serial.println("PWM Filter");
  pinMode(pwmPin, OUTPUT);
  analogWrite(pwmPin, 0);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, 0);
  pinMode(vPin, INPUT);
}

void SetPWM(int pwm)
{
  analogWrite(pwmPin, pwm);
  delay(writeStabilisation);
}

float Read()
{
  digitalWrite(ledPin, 1);
  //delay(readStabilisation);
  int v = analogRead(vPin);
  digitalWrite(ledPin, 0);  
  return (float)v*5.0/1024.0; 
}

void loop() 
{
  for(int i = 0; i < 256; i++)
    {
      SetPWM(i);
      Serial.print(i);
      Serial.print(", ");
      Serial.println(Read());
    }
    
    while(1);
/*  if(saw)
  {
    for(int i = 110; i < 256; i++)
    {
      analogWrite(pwmPin, i);
      delay(1);
    }
  }
  
  if(Serial.available() > 0)
  {
    int v = Serial.parseInt();
    if(v >= 0)
    {
      //saw = false;
      Serial.print("Setting ");
      Serial.println(v);
      analogWrite(pwmPin, v);
    } else
    {
      led = 1 - led;
      digitalWrite(ledPin, led);    
    }
    while(Serial.available() > 0);
  }
  */
}

