int op0 = 6;
int set = 3;
int clk = 4;
int d = 2;

void Reset()
{
  digitalWrite(set, 1);
  digitalWrite(set, 0);
  digitalWrite(set, 1);
}

void Clock()
{
  digitalWrite(clk, 1);
  digitalWrite(clk, 0);
  digitalWrite(clk, 1);
}

void Set(int n)
{
  Reset();
  digitalWrite(d, 0);
  Clock();
  digitalWrite(d, 1);
  for(int i = 0; i < n; i++)
   Clock();
}

int PrintState()
{
  Serial.print("\nState: ");
  int state = 0;
  for(int i = 5; i >= 0; i--)
  {
    state << 1;
    if(digitalRead(op0 + i))
    {
      state |= 1;
      Serial.print(1);
    } else
    {
      Serial.print(0);   
    }
  }
  Serial.println();
  return state;
}

int ReadState()
{
  int state = -1;
  int zCount = 0;
  for(int i = 5; i >= 0; i--)
  {
    if(!digitalRead(op0 + i))
    {
      state = i;
      zCount++;
    }
  }
  if(zCount != 1)
    state = -1;
  return state;
}

void setup() 
{
  Serial.begin(9600);
  for(int i = 0; i < 6; i++)
  {
    pinMode(op0 + i, INPUT);
  }
  pinMode(set, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(d, OUTPUT);
  digitalWrite(set, 1);
  digitalWrite(clk, 1);
  digitalWrite(d, 1);
  Reset();
  Serial.println("\n\nShift register test");
  PrintState();
  for(int i = 0; i < 7; i++)
  {
    Set(i);
    PrintState();
  }
}

long loopCount = 0;
long errorCount = 0;

void loop()
{
  int v0 = random(6);
  //Serial.println(v0);
  Set(v0);
  int v1 = ReadState();
  if(v1 != v0)
    errorCount++;
  loopCount++;
  if(!(loopCount % 100000))
  {
    Serial.print("\nTests: ");
    Serial.print(loopCount);
    Serial.print(", errors: ");
    Serial.println(errorCount);
  }

}
