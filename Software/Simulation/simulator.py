'''

Simulator for the RepRap Ltd Optical Neural Network

Adrian Bowyer

RepRap Ltd
http://reprapltd.com

5 November 2020

Licence: GPL

---------------------------------------------------------

This simulator is a bit more complicated than a conventional
neural network because it works with the actual voltages and
light levels that the real network would use.

See: https://reprapltd.com/a-slow-write-fast-read-optical-artificial-neural-network-part-1/


'''


import random
import math

def Ran8():
 return random.randint(0, 255)

def RanBool():
 return Ran8()%2 is 1

# Empirical data

s1 = [4.38, 4.62, 4.68, 4.71, 4.73, 4.74, 4.74, 4.75, 4.76, 4.77, 4.77, 4.77, 4.77, 4.77, 4.78, 4.77]
s2 = [4.27, 4.51, 4.58, 4.62, 4.64, 4.66, 4.67, 4.68, 4.69, 4.69, 4.7, 4.7, 4.7, 4.71, 4.71, 4.71]
s4 = [4.53, 4.63, 4.67, 4.71, 4.72, 4.74, 4.74, 4.75, 4.76, 4.77, 4.77, 4.77, 4.78, 4.78, 4.78, 4.78]
s8 = [4.46, 4.62, 4.67, 4.7, 4.71, 4.73, 4.74, 4.75, 4.75, 4.76, 4.76, 4.77, 4.77, 4.77, 4.77, 4.77]
pwmIncrement = 17

# A single synapse.  

# The lookup table is the output voltage for a given UV LED pwm.
# The synapse has two states: firing and quiet, and outputs two voltages that
# correspond to those states.  In the real machine those voltages would be the
# result of the synapse's visible LED firing and its light being directed through 
# a photochromic filter to a phototransistor in a neuron (see below).  When the synapse 
# is not being used its photochromic filter is kept at a fixed darkness by the synapse's UV led 
# shining on it with the pulse-width-modulated value stored in the synapse.

class Synapse:

 def __init__(self, v = [0, 1], pwmIncrement = 255, pwm = 0):
  self.lookUpVolts = []
  for vv in v:
   self.lookUpVolts.append(vv - v[0])
  self.pwmIncrement = pwmIncrement
  self.pwm = pwm
  self.outputFiring = self.LookUp(self.pwm)
  self.outputQuiet = self.LookUp(255)       # Dark is high voltage, active is low
  self.firing = False

 def LookUp(self, pwm):
  lower = math.floor(pwm/self.pwmIncrement)
  #print(str(pwm) + ", " + str(lower))
  if(lower > len(self.lookUpVolts) - 2):
   lower = len(self.lookUpVolts) - 2
  v0 = self.lookUpVolts[lower]
  v1 = self.lookUpVolts[lower+1]
  pwm0 = lower*self.pwmIncrement + 0.0
  pwm1 = (lower+1)*self.pwmIncrement + 0.0
  return v0 + (v1 - v0)*(pwm - pwm0)/(pwm1 - pwm0)

 def Output(self):
  if self.firing:
   return self.outputFiring
  else:
   return self.outputQuiet

 def SetPWM(self, pwm):
  self.pwm = pwm
  self.outputFiring = self.LookUp(self.pwm)
  self.outputQuiet = self.LookUp(255)

 def SetFiring(self):
  self.firing = True

 def SetQuiet(self):
  self.firing = False

 def IsFiring(self):
  return self.firing


# A single neuron

# The neuron sums the inputs from all its dendritic excitatory synapses and subtracts the
# sum of all its dendritic inhibitory synapses.  If the total exceeds a threshold, the neuron fires.
# When it fires it updates the state of all the synapses on its output axon.
# In the real machine the sums are done in parallel by two phototransistors arranged in a 
# long-tailed pair so that one set of inputs is subtracted from the other set, resulting in a voltage.
# The voltage is fed into a comparator that subtracts a threshold from it.  If the sum
# is greater than the threshold the neuron fires, turning on the visible LEDs in all
# the synapses connected to its output axon.
 
class HalfNeuron:

 def __init__(self, lowestVoltage = 2.55, noInput = 5.0):
  self.inputSynapses = []
  self.volts = lowestVoltage
  self.lowestVoltage = lowestVoltage
  self.noInput = noInput

 def AddInput(self, synapse):
  self.inputSynapses.append(synapse)

 def Output(self):
  return self.volts

 def Run(self):
  self.volts = self.lowestVoltage
  allZero = True
  for synapse in self.inputSynapses:
   if synapse.IsFiring():
    allZero = False
   self.volts += synapse.Output()
  if allZero:
   self.volts = self.noInput

# Testing single neuron to see if it matches experiment...

def SetNumber(n, halfNeuron):
 count = 0
 for synapse in halfNeuron.inputSynapses:
  if ((n >> count) & 1) is 1:
   synapse.SetFiring()
  else:
   synapse.SetQuiet()
  count += 1

def PrintInputScan(halfNeuron):
 for synapse in halfNeuron.inputSynapses:
  print(str(synapse.pwm) + ", ", end="")
 for n in range(16):
  SetNumber(n, halfNeuron)
  halfNeuron.Run()
  print(f'{halfNeuron.Output():.2f}' + ", ", end ="")
 print()

def BuildHalfNeuron():
 halfNeuron = HalfNeuron()
 halfNeuron.AddInput(Synapse(s1, pwmIncrement, pwm = 0))
 halfNeuron.AddInput(Synapse(s2, pwmIncrement, pwm = 0))
 halfNeuron.AddInput(Synapse(s4, pwmIncrement, pwm = 0))
 halfNeuron.AddInput(Synapse(s8, pwmIncrement, pwm = 0))
 return halfNeuron

def CallibrationExperiment():
 halfNeuron = BuildHalfNeuron()
 for synapse in halfNeuron.inputSynapses:
  synapse.SetPWM(0)
 for synapse in halfNeuron.inputSynapses:
  for pwm in range(0, 255, 17):
   PrintInputScan(halfNeuron)
   synapse.SetPWM(synapse.pwm + 17)

def LossFunction(halfNeuron, threshold):
 wrong = 0.0
 for n in range(16):
  SetNumber(n, halfNeuron)
  halfNeuron.Run()
  v = halfNeuron.Output()
  error = False
  if (n & 1) is 1:
   if v > threshold:
    error = True
  else:
   if v < threshold:
    error = True
  d = threshold - v
  if error:
   wrong += d*d
 return wrong

def OddEvenTest1(threshold):
 halfNeuron = BuildHalfNeuron()
 halfNeuron.inputSynapses[0].SetPWM(0)
 for s in range(1, 4):
  halfNeuron.inputSynapses[s].SetPWM(100)
 for n in range(16):
  SetNumber(n, halfNeuron)
  halfNeuron.Run()
  v = halfNeuron.Output()
  if v > threshold:
   print(str(n) + " is even. ", end='')
  else:
   print(str(n) + " is odd. ", end='')
  print(v)
 print("Loss: " + str(LossFunction(halfNeuron, threshold)))


OddEvenTest1(3.8)
#CallibrationExperiment()

'''
random.seed(17)
n = Network()
tt = 0
ft = 0
for k in range(1):
 i = []
 for x in range(4):
  j = []
  for y in range(4):
   b = RanBool()
   j.append(b)
  i.append(j)
 #PrintPattern(i)
 n.SetInputs(i)
 o = n.Run()
 n.PrintState()
 if o:
  tt += 1
 else:
  ft += 1
 print("Network output: " + str(o))

print("Trues: " + str(tt) + ", falses: " + str(ft))


random.seed(17)
s = Synapse(200)
s.SetRandomPWM()
s.SetQuiet()
print(s.Output())
s.SetFiring()
print(s.Output())

def PrintPattern(i):
 for x in range(4):
  for y in range(4):
   if i[x][y]:
    print('1' + ' ', end='')
   else:
    print('0' + ' ', end='')
  print()



# The entire network

# At the moment this is set up as a 4 x 4 array of inputs into 4 8-input neurons.
# 4 of each input are excitation inputs, and 4 are inhibiters.
# The 4 outputs of those are fed into a fifth and final output neuron.

class Network:
 def __init__(self):
  self.neurons = []
  for x in range(5):
   self.neurons.append(HalfNeuron())

  self.inputExciters = []
  self.inputInhibitors = []
  self.intermediateExciters = []
  self.intermediateInhibitors = []
  for x in range(4):
   ise = []
   isi = []
   se = Synapse(Ran8())
   si = Synapse(Ran8())
   #print('Adding intermediate synapses (' + str(x) + ') to neuron 4')
   self.neurons[4].AddInput(se)
   self.neurons[4].AddInhibitor(si)
   self.intermediateExciters.append(se)
   self.intermediateInhibitors.append(si)
   self.neurons[x].AddOutput(se)
   self.neurons[x].AddOutput(si)
   for y in range(4):
    se = Synapse(Ran8())
    ise.append(se)
    si = Synapse(Ran8())
    isi.append(si)
    n = (4*x + y)%4
    #print('Adding e and s synapses (' + str(x) + ', ' + str(y) + ') to neuron ' + str(n))
    self.neurons[n].AddInput(se)
    self.neurons[n].AddInhibitor(si)
   self.inputExciters.append(ise)
   self.inputInhibitors.append(isi)
  self.neurons[4].AddOutput(Synapse(Ran8()))

 def SetInputs(self, i):
  for x in range(4):
   for y in range(4):
    if i[x][y]:
     self.inputExciters[x][y].SetFiring()
     self.inputInhibitors[x][y].SetFiring()
    else:
     self.inputExciters[x][y].SetQuiet()
     self.inputInhibitors[x][y].SetQuiet()

 def PrintState(self):
  print("\nExcitation inputs: ")
  for x in range(4):
   print(' ', end = '')
   for y in range(4):
    if self.inputExciters[x][y].IsFiring():
     print('1' + ' ', end='')
    else:
     print('0' + ' ', end='')
   print()
  print("Inhibition inputs: ")
  for x in range(4):
   print(' ', end = '')
   for y in range(4):
    if self.inputInhibitors[x][y].IsFiring():
     print('1' + ' ', end='')
    else:
     print('0' + ' ', end='')
   print()
  print("Intermediate exciter inputs: ")
  for x in range(4):
   print(' ', end = '')
   if self.intermediateExciters[x].IsFiring():
    print('1' + ' ', end='')
   else:
    print('0' + ' ', end='')
  print()
  print("Intermediate inhibitor inputs: ")
  for x in range(4):
   print(' ', end = '')
   if self.intermediateInhibitors[x].IsFiring():
    print('1' + ' ', end='')
   else:
    print('0' + ' ', end='')
  print()
  print("Output: " + str(self.neurons[4].outputs[0].IsFiring()) + "\n\n")

 def Run(self):
  for n in range(5):
   self.neurons[n].Run()
  return self.neurons[4].outputs[0].IsFiring()
'''
