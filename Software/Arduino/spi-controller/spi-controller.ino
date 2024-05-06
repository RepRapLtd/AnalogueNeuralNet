


#include <SPI.h>

int pin0 = 2;
int debug =0;

SPISettings mySetting(1000000, LSBFIRST, SPI_MODE1);

// Send a 32 bit data packet out on the SPI bus

void Send32Bits(uint8_t b[])
{
  digitalWrite(10, LOW);            // Select the chip
  delay(1);
  SPI.beginTransaction(mySetting);
  for(int i = 0; i < 4; i++)
  {
    SPI.transfer(b[i]);
  }
  SPI.endTransaction();
  delay(1);
  digitalWrite(10, HIGH);           // Deselect the chip
  if(debug)
  {
    Serial.print("Bytes sent: ");
    for(int i = 0; i < 4; i++)
    {
      Serial.print(b[i], HEX);
      Serial.print(", ");
    }
    Serial.println();
  }  
}

void SendCommand(int command, int address, uint16_t data)
{
  uint8_t b[4];  
  b[0] = command & 0xF;
  b[1] = (address & 0xF) << 4;
  b[1] = b[1] | ((data >> 12) & 0xF);
  b[2] = (data >> 4) & 0xFF;
  b[3] = (data & 0xF) << 4;
  Send32Bits(b);
}



void SetVoltage(int address, float v)
{
  uint16_t data = 0;
  if(v >= 0.0 && v <= 1.0)
  {
    data = uint16_t(v*65535.0);
    SendCommand(0x3, address, data);
  } else
  {
    Serial.print("Voltage out of range: ");
    Serial.println(v);
  }
}

void startSPI()
{
// GPT4 says send:
// 0x4F000000
// 0x2F000001
// 0x7F800000
  uint8_t b[4];
  b[0] = 0x4F;
  b[1] = 0;
  b[2] = 0;
  b[3] = 0;
//  Send32Bits(b);
  b[0] = 0x2F;
  b[1] = 0x00;
  b[2] = 0x00;
  b[3] = 0x01;
//  Send32Bits(b); 
  b[0] = 0x7F;
  b[1] = 0x80;
  b[2] = 0;
  b[3] = 0;
//  Send32Bits(b); 
// Data sheet says send:
  SendCommand(0x7, 0, 0);
//  delay(1);
  SendCommand(0x8, 0xF, 0xFFFF);
  SendCommand(0x5, 0, 0);
  SendCommand(0x4, 0x0, 0);
}

void setVoltages(float v)
{
  for(int address = 0; address < 8; address++)
  {
    SetVoltage(address, 1.0);
  }  
}

void setup()
{
  pinMode(10, OUTPUT); // set the SS pin as an output
  digitalWrite(10, HIGH);
  SPI.begin();         // initialize the SPI library
  Serial.begin(9600);
  for (int i = 0; i < 4; i++)
  {
    pinMode(pin0 + i, OUTPUT);
    digitalWrite(pin0 + i, HIGH);
  }
  startSPI();
  if(debug)
    Serial.println("\nSPI controller");
}

void loop()
{
  if(Serial.available())
  {
    char c = Serial.read();
    if(c != '\n')
      setVoltages(0.5);
  }
}
