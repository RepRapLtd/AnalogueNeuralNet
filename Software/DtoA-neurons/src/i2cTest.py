import i2cdriver

debug = True
vMult = 2.485

def realToBytes(value):
    # Ensure the value is within the allowed range
    if 0.0 <= value <= 1.0:
        # Convert to a 16-bit unsigned integer
        int_value = int(value * 65535)
        # Extract the least significant byte (LSB) and most significant byte (MSB)
        lsb = int_value & 0xFF
        msb = (int_value >> 8) & 0xFF
        return lsb, msb
    else:
        raise ValueError("realToBytes() - Value must be between 0.0 and 1.0.")

class i2cControl:

  def setAtoD(self, bs):
    self.i2c.start(self.atod, 0)
    self.i2c.write(bs)
    self.i2c.stop()

  def __init__(self):
    self.i2c = i2cdriver.I2CDriver("/dev/ttyUSB0")
    self.atod = 0x57
    self.dip = 0x4
    self.excite = [3, 2, 1, 0]
    self.inhibit = [7, 6, 5, 4]
    self.voltages = [0,0,0,0,0,0,0,0]
    self.bits = [0,0,0,0]
    bs = [0x70,0,0] # reset
    self.setAtoD(bs)
    bs=[0x80,0,1] # internal ref voltage
    self.setAtoD(bs)
    bs=[0x40,0,0xff] # Activate all channels
    self.setAtoD(bs)

  def setVoltage(self, channel, voltage):
    if channel >= 0 or channel <= 7:
        lsb, msb = realToBytes(voltage)
        bs = [0x30 | channel, msb, lsb]
        self.setAtoD(bs)
        self.voltages[channel] = voltage*vMult
    else:
        raise ValueError("setVoltage() - channel must be be between 0 and 7.")

  def setInhibition(self, i):
      for k in range(4):
          self.setVoltage(self.inhibit[k], i[k])

  def setExitation(self, e):
      for k in range(4):
          self.setVoltage(self.excite[k], e[k])

  def setInputBits(self, b):
    self.i2c.start(self.dip, 0)
    self.i2c.write([b])
    self.i2c.stop()
    for i in range(4):
        if b & 1:
            self.bits[i] = 1
        else:
            self.bits[i] = 0
        b = b >> 1

  def printState(self):
      print("I2C scan:")
      self.i2c.scan()
      print("Input bits: ", end='')
      for i in range(4):
          print(str(self.bits[i]) + ", ", end='')
      print()
      print("Inhibition voltages: ", end='')
      iSum = 0.0
      for i in range(4):
          v = self.voltages[self.inhibit[i]]
          print(str(v) + ", ", end='')
          if self.bits[i] == 1:
              iSum = iSum + v
      print("sum = " + str(iSum))
      print("Exitation voltages: ", end='')
      eSum = 0.0
      for i in range(4):
          v = self.voltages[self.excite[i]]
          print(str(v) + ", ", end='')
          if self.bits[i] == 1:
              eSum = eSum + v
      print("sum = " + str(eSum))



ic = i2cControl()
ic.setInputBits(0xf)
#ic.setInhibition([1.0,1.0,1.0,1.0])
#ic.setExitation([1.0,1.0,1.0,1.0])
ic.setInhibition([0.5,0.5,0.5,0.5])
ic.setExitation([0.5,0.5,0.5,0.5])
ic.printState()
#for i in range(8):
#   ic.setVoltage(i, 1.0)


