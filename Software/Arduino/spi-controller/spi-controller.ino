


#include <SPI.h>

int pin0 = 2;
int debug = 1;

SPISettings mySetting(1000000, MSBFIRST, SPI_MODE0);

// Send a 32 bit data packet out on the SPI bus

void Send32Bits(int command, int address, uint16_t data)
{
  digitalWrite(10, LOW);            // Select the chip
  SPI.beginTransaction(mySetting);
  uint8_t b[4];  
  b[0] = command & 0xF;
  b[1] = (address & 0xF) << 4;
  b[1] = b[1] | ((data >> 12) & 0xF);
  b[2] = (data >> 4) & 0xFF;
  b[3] = (data & 0xF) << 4;
  for(int i = 0; i < 4; i++)
  {
    SPI.transfer(b[i]);
  }
  digitalWrite(10, HIGH);           // Deselect the chip
  SPI.endTransaction();
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



void SetVoltage(int address, float v)
{
  uint16_t data = 0;
  if(v >= 0.0 && v <= 1.0)
  {
    data = uint16_t(v*65535.0);
    Send32Bits(0x3, address, data);
  } else
  {
    Serial.print("Voltage out of range: ");
    Serial.println(v);
  }
}

void setup()
{
  pinMode(10, OUTPUT); // set the SS pin as an output
  digitalWrite(10, HIGH);
  SPI.begin();         // initialize the SPI library
  Serial.begin(9600);
  for (int i = 3; i > 0; i--)
  {
    pinMode(pin0 + i, OUTPUT);
    digitalWrite(pin0 + i, HIGH);
  }
  Serial.println("\nSPI controller");
  Send32Bits(0x7, 0, 0);
  Send32Bits(0x8, 0xF, 0xFFFF);
  Send32Bits(0x4, 0x0, 0);
  for(int address = 0; address < 8; address++)
  {
    SetVoltage(address, 0.5);
  }
}

void loop()
{
}
