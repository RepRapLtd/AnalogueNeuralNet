# Modular Neuromorphic Analogue Neural Net

![Synapse Test](https://github.com/RepRapLtd/AnalogueNeuralNet/blob/main/Pix/synapse-breadboard.jpg)

This is a project to create a neuromorphic analoge neural network. It will be modular, and so can be used to make networks of any size (within limits yet to be explored).

The basic idea is to use crosspoint analoge switches to make all the interconnections in a way that is completely programmable.

So. How will it work? (Chip numbers in brackets are suggestions, and may not form part of the final design.)

![Neuron circuit](https://github.com/RepRapLtd/AnalogueNeuralNet/blob/main/Electronics/Diagrams/crosspoint/crosspoint-One%20neuron.png)

This is the central idea for a single artificial neuron.There are a number of digital inputs I0+...In+ and I0-...In+. Each controls an electronic analogue switch (SN74LVC1G3157) that selects either GROUND or a voltage, V. The voltages are set by digital to analogue converters (AD5629R) and are the weights in the network. There are exitatory inputs (I+) and inhibitory ones (I-). They are all summed by two operational amplifiers (MCP6004) the voltages from which are fed into a comparitor to give the final output, O.

The neuron has fired when O = 1, and not when O = 0.

The photograph above shows a test breadboard of this design using the chips listed. The D to A converter and the Arduino are both controlled by the <a href="https://i2cdriver.com/" target="_blank">I2C driver</a> in the middle, which allows the whole thing to be run via a single USB from a Python program running on a PC. The Arduino is supplying the inputs, I, and is reading and reporting the output voltage from the summing op. amp.

So far, so (I hope) so straightforward. It's an analogue version of the way that a conventional artificial neural network operates, but it's a lot closer to the biological original than linearising the thing in a program for a Turing-equivalent machine.

But I obviously don't want to hard-wire the connections. I want the input data to be routed to any I input; I want the resulting weights/voltages to be routable in any combination to be summed; and I want any output, O, to be able to feed into any group of inputs, I in the next layer (or to form a bit of the final result). To do this I will use 8x8 crosspoint analogue switches (ADG2188).

First, here is how the voltages will be connected to the summing amplifiers:

![Synapse to axon connections](https://github.com/RepRapLtd/AnalogueNeuralNet/blob/main/Electronics/Diagrams/crosspoint/crosspoint-Synapse%20to%20axon%20connections.png)

Only one analogue switch in any row can be closed, but any number in a column can be closed. The closed switches in a column cause the corresponding voltages selected by the inputs, I, to be summed. This happens in adjacent pairs, one (left) exitatory, and one (right) inhibitory. The inputs on the left are labelled "synapses", but it is perhaps more accurate to regard a single column of switches to be synapses.

So now there are a collection of outputs, O. These have to be connected to the inputs of the next layer, or form part of the final result:

![Axon to synapse connections](https://github.com/RepRapLtd/AnalogueNeuralNet/blob/main/Electronics/Diagrams/crosspoint/crosspoint-Axon%20to%20synapse%20connections.png)

The switch matrix works in exactly the same way as before (though there are different numbers of rows and columns).


Adrian Bowyer
7 May 2024


