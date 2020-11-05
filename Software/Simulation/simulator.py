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

# A single synapse.  

# The lookup table is the output voltage for a given UV LED pwm.
# The synapse has two states: firing and quiet, and outputs two voltages that
# correspond to those states.  In the real machine those voltages would be the
# result of the synapse's visible LED firing and its light being directed through 
# a photochromic filter to a phototransistor in a neuron (see below).  When the synapse 
# is not being used its photochromic filter is kept at a fixed darkness by the synapse's UV led 
# shining on it with the pulse-width-modulated value stored in the synapse.

class Synapse:

 def __init__(self, pwm):
  self.lookUpPWM = [0, 63, 127, 191, 255]
  self.lookUpVolts = [3.2, 3.8, 3.9, 4.1, 4.2]
  self.SetPWM(pwm)
  self.firing = False

 def LookUp(self, pwm):
  for i in range(1, len(self.lookUpPWM)):
   if self.lookUpPWM[i] >= pwm:
    v0 = self.lookUpVolts[i-1]
    v1 = self.lookUpVolts[i]
    pwm0 = self.lookUpPWM[i-1] + 0.0
    pwm1 = self.lookUpPWM[i] + 0.0
    return v0 + (v1 - v0)*(pwm - pwm0)/(pwm1 - pwm0)
  print("Synapse look-up error!")
  return self.lookUpVolts[0]

 def Output(self):
  if self.firing:
   return self.outputFiring
  else:
   return self.outputQuiet

 def SetPWM(self, pwm):
  self.pwm = pwm
  self.outputFiring = self.LookUp(self.pwm)
  self.outputQuiet = self.LookUp(0)

 def SetRandomPWM(self):
  self.SetPWM(random.randint(0, 255))

 def SetFiring(self):
  self.firing = True

 def SetQuiet(self):
  self.firing = False

 def IsFiring(self):
  return self.firing


# A single neuron

# The neuron sums the inputs from all its dendritic excitatory synapses and subtracts the
# sum of all its dendritic inhibitory synapses.  If the total exceeds a threshold, the neuron fires.
# When it fires it ubdates the state of all the synapses on its output axon.
# In the real machine the sums are done in parallel by two phototransistors arranged in a 
# long-tailed pair so that one set of inputs is subtracted from the other set, resulting in a voltage.
# The voltage is fed into a comparator that subtracts a threshold from it.  If the sum
# is greater than the treshold the neuron fires, turning on the visible LEDs in all
# the synapses connected to its output axon.
 
class Neuron:

 def __init__(self, threshold):
  self.exciters = []
  self.inhibitors = []
  self.outputs = []
  self.SetThreshold(threshold)

 def AddExciter(self, synapse):
  self.exciters.append(synapse)

 def AddInhibitor(self, synapse):
  self.inhibitors.append(synapse)

 def AddOutput(self, synapse):
  self.outputs.append(synapse)

 def SetThreshold(self, threshold):
  self.threshold = threshold

 def Run(self):
  sum = 0.0
  for synapse in self.exciters:
   sum += synapse.Output()
  for synapse in self.inhibitors:
   sum -= synapse.Output()
  firing = (sum >= self.threshold)    
  for synapse in self.outputs:
   if firing:
    synapse.SetFiring()
   else:
    synapse.SetQuiet()



random.seed(17)
s = Synapse(200)
s.SetRandomPWM()
s.SetQuiet()
print(s.Output())
s.SetFiring()
print(s.Output())
