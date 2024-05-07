# Modular Neuromorphic Analogue Neural Net

![Synapse Test](https://github.com/RepRapLtd/AnalogueNeuralNet/blob/main/Pix/synapse-breadboard.jpg)

This is a project to create a neuromorphic analoge neural network. It will be modular, and so can be used to make networks of (within limits yet to be explored) any size.

The basic idea is to use crosspoint analoge switches to make all the interconnections in a way that is completely programmable.

So. How will it work? (Chip numbers in brackets are suggestions, and may not form part of the final design.)

![](https://github.com/RepRapLtd/AnalogueNeuralNet/blob/main/Electronics/Diagrams/crosspoint/crosspoint-One%20neuron.png)

This is the central idea for a single artificial neuron.There are a number of digital inputs I0+...In+ and I0-...In+. Each controls an electronic analogue switch (SN74LVC1G3157) that selects either GROUND or a voltage, V. The voltages are set by digital to analogue converters (AD5629R) and are the weights in the network. There are exitatory inputs (I+) and inhibitory ones (I-). They are all summed by two operational amplifiers (MCP6004) the voltages from which are fed into a comparitor to give the final out put O.

The neuron has fired when O = 1, and not when O = 0.

The photograph above shows a test breadboard of this design using the chips listed. The D to A converter and the Arduino are both controlled by the <a href="https://i2cdriver.com/">I2C driver</a> in the middle, which allows the whole thing to be run via USB from a Python program running on a PC. The Arduino is supplying the inputs, I, and is reading and reporting the output voltage from the summing op. amp.


