# Simulation of the crosspoint neural network architecture
#
# This program builds, trains, and uses neural networks of
# different sizes for different tasks. It's classes mirror the
# electronics for the crosspoint neural network, and so it
# will also be able to download its taught patterns of weights
# into that hardware.
#
# Written by Adrian Bowyer and GPT-4o
#
# RepRap Ltd
# https://reprapltd.com
#
# Licence: GPL
#

import numpy as np

# Neurons drive other neurons via synapses. This is the synapse class. When a neuron
# fires with synapses as its outputs this sends its weight value to the neuron
# that it drives, which is the argument to its initialization function. Synapses can
# be excitatory or inhibitory.

class Synapse:
    def __init__(self, neuron):
        self.weight = np.random.uniform(0.0, 1.0)  # Initialize with a random weight
        self.neuron = neuron
        self.is_excitatory = np.random.choice([True, False])
        if self.is_excitatory:
            neuron.excitatory_count += 1
        else:
            neuron.inhibitory_count += 1

    def set_weight(self, weight):
        if 0.0 <= weight <= 1.0:
            self.weight = weight
        else:
            raise ValueError("Weight must be between 0 and 1")

    def transmit(self):
        if self.is_excitatory:
            self.neuron.receive_excitation(self.weight)
        else:
            self.neuron.receive_inhibition(self.weight)

    def set_type(self, is_excitatory):
        if self.is_excitatory != is_excitatory:
            self.neuron.update_synapse_count(is_excitatory)
            self.is_excitatory = is_excitatory


# This is the class for a single neuron. Neurons are in layers, and they record which layer they're in
# and their index in that layer for administrative purposes. These values take no part in the network when
# it's running. Each neuron has a list of synapses that it drives when it fires. It also records excitations
# and inhibitions - these are the sum of the inputs from the synapses that drive this neuron. If excitations
# exceed inhibitions the neuron fires. The neuron also knows how many excitatory and inhibitory inputs
# it has. It uses these numbers to average the excitations and inhibitions numbers, because this is
# the way that the electronics being modelled works.

class Neuron:
    def __init__(self, layer_index, neuron_index):
        self.layer_index = layer_index
        self.neuron_index = neuron_index
        self.synapses = []
        self.excitations = 0.0
        self.inhibitions = 0.0
        self.output = 0.0  # Output value after activation
        self.delta = 0.0  # Used for backpropagation
        self.excitatory_count = 0
        self.inhibitory_count = 0

    def add_synapse(self, synapse):
        self.synapses.append(synapse)

    def receive_excitation(self, amount):
        self.excitations += amount

    def receive_inhibition(self, amount):
        self.inhibitions += amount

    def update_synapse_count(self, is_excitatory):
        if is_excitatory:
            self.excitatory_count += 1
            self.inhibitory_count -= 1
        else:
            self.excitatory_count -= 1
            self.inhibitory_count += 1
        if self.excitatory_count < 0 or self.inhibitory_count < 0:
            raise ValueError(f"excitatory_count {self.excitatory_count} or inhibitory_count {self.inhibitory_count} has gone negative.")

    # This is the function that sees if this neuron fires, and - if it does - sends signals
    # along all its synapses to the neurons it drives.

    def check_fire(self):
        if self.excitatory_count > 0:
            plus = self.excitations/self.excitatory_count
        else:
            plus = 0.0
        if self.inhibitory_count > 0:
            minus = self.inhibitions/self.inhibitory_count
        else:
            minus = 0.0
        self.output = plus > minus
        #if self.layer_index == 2:
        #    print(f"plus: {plus}, minus: {minus}")
        if self.output:
            for synapse in self.synapses:
                synapse.transmit()

    def reset(self):
        self.excitations = 0.0
        self.inhibitions = 0.0
        self.output = False
        self.delta = 0.0


# This is the class that models the whole network. It is initialised with an array like
# [3, 5, 2], which means that there are three input neurons, five in a hidden layer, and
# 2 outputs. There can be any number of hidden layers. One extra "hidden" neuron is added
# to the input layer that always fires. This allows the network to produce a defined
# output when there is a zero input.

class Network:
    def __init__(self, layers):
        self.layers = layers
        self.neurons = []
        self.synapses = []
        self.create_network()

    def create_network(self):
        # Create neurons layer by layer
        for layer_index, layer_size in enumerate(self.layers):
            # The hidden neuron in layer 0 that's always on.
            if layer_index == 0:
                n = Neuron(layer_index, -1)
                n.excitatory_count = 1
                layer = [n]
            else:
                layer = []
            for neuron_index in range(layer_size):
                n = Neuron(layer_index, neuron_index)
                # Input layer neurons get data from the World, and those data are excitatory.
                if layer_index == 0:
                    n.excitatory_count = 1
                layer.append(n)
            self.neurons.append(layer)

        # Create synapses between consecutive layers
        for i in range(len(self.layers) - 1):
            current_layer = self.neurons[i]
            next_layer = self.neurons[i + 1]
            layer_synapses = []
            for neuron in current_layer:
                for next_neuron in next_layer:
                    synapse = Synapse(neuron=next_neuron)
                    neuron.add_synapse(synapse)
                    layer_synapses.append(synapse)
            self.synapses.append(layer_synapses)

# Rescale all the weights so that they lie between 0 and 1.
    def normalise_weights(self):
       # Rescale weights to lie between 0 and 1
        max_weight = float('-inf')
        for layer in self.synapses:
            for synapse in layer:
                max_weight = max(max_weight, abs(synapse.weight))
        scale_factor = 1.0 / max_weight
        for layer in self.synapses:
            for synapse in layer:
                synapse.set_weight(synapse.weight * scale_factor)
    def reset_network(self):
        for layer in self.neurons:
            for neuron in layer:
                neuron.reset()

    def check_fire_all(self):
        for layer in self.neurons:
            for neuron in layer:
                neuron.check_fire()

    def propagate(self, input_array, epoch=None):
        if len(input_array) != len(self.neurons[0]) - 1:
            raise ValueError(f"Input array length ({len(input_array)}) must match the number of input neurons ({len(self.neurons[0])}).")

        # Reset the network before propagation
        self.reset_network()

        # First input is always true
        self.neurons[0][0].receive_excitation(1.0)

        # Fire the input neurons corresponding to TRUE in the input array
        for i, state in enumerate(input_array):
            if state:
                self.neurons[0][i+1].receive_excitation(1.0)  # Assuming maximum excitation

        # Propagate firing through the network
        self.check_fire_all()

        # Get the state of the output layer
        output_layer = self.neurons[-1]
        output_states = [neuron.output for neuron in output_layer]

        return output_states

    def backpropagate(self, desired_output, learning_rate = 0.01):
        if len(desired_output) != len(self.neurons[-1]):
            raise ValueError(f"Desired output length ({len(desired_output)}) must match the number of output neurons ({len(self.neurons[-1])}).")

        # Calculate output layer deltas
        for neuron_index, neuron in enumerate(self.neurons[-1]):
            neuron.delta = (1.0 if desired_output[neuron_index] else 0.0) - (1.0 if neuron.output else 0.0)

        # Backpropagate the error to hidden layers

        for layer in range(len(self.layers) - 2, -1, -1):
            for neuron_index, neuron in enumerate(self.neurons[layer]):
                plus = 0.0
                minus = 0.0
                for synapse in neuron.synapses:
                    if synapse.is_excitatory:
                        plus += synapse.weight * synapse.neuron.delta
                    else:
                        minus += synapse.weight * synapse.neuron.delta
                if synapse.neuron.excitatory_count > 0:
                        plus = plus/synapse.neuron.excitatory_count
                else:
                    plus = 0
                if synapse.neuron.inhibitory_count > 0:
                        minus = minus/synapse.neuron.inhibitory_count
                else:
                    minus = 0
                neuron.delta = (plus - minus)*(neuron.excitations - neuron.inhibitions)

        # Update the weights and potentially change the synapse type
        for layer in range(len(self.layers) - 1):
            for neuron in self.neurons[layer]:
                for synapse in neuron.synapses:
                    weight_change = learning_rate * (1.0 if neuron.output else 0.0) * synapse.neuron.delta
                    if synapse.is_excitatory:
                        new_weight = synapse.weight + weight_change
                    else:
                        new_weight = -synapse.weight + weight_change
                    # Set the type if necessary
                    if new_weight < 0:
                        synapse.set_type(False)  # Set to inhibitory
                    else:
                        synapse.set_type(True)  # Set to inhibitory
                    synapse.weight = abs(new_weight)
        self.normalise_weights()


# Network as a printable string
    def network_to_string(self):
        result = ""
        for layer_index, layer in enumerate(self.neurons):
            result += f"Layer {layer_index}:\n"
            for neuron in layer:
                result += f"  Neuron {neuron.neuron_index}, excitatory_count: {neuron.excitatory_count}, inhibitory_count: {neuron.inhibitory_count}, "
                result += f"delta: {neuron.delta}, excitations: {neuron.excitations}, inhibitions: {neuron.inhibitions}:\n"
                for synapse in neuron.synapses:
                    result += f"    -> Neuron {synapse.neuron.layer_index}-{synapse.neuron.neuron_index} (Weight: {synapse.weight:.4f}, {'Excitatory' if synapse.is_excitatory else 'Inhibitory'})\n"
        return result


# Function to test training and evaluating a network
def train_and_evaluate(layers, learning_rate, epochs, report):

    # Generate random training data
    training_data = {i: [bool(int(x)) for x in f'{np.random.randint(0, 4):02b}'] for i in range(8)}

    # Initialize the network
    network = Network(layers)

    # Train the network on the training data
    for epoch in range(epochs):
        for input_value, desired_output in training_data.items():
            input_array = []
            k = input_value
            for i in range(3):
                input_array.append(bool(k & 1))
                k = k >> 1
            network.propagate(input_array)
            network.backpropagate(desired_output, learning_rate)

    # Evaluate the network on the training data
    correct_count = 0
    for input_value, desired_output in training_data.items():
        input_array = []
        k = input_value
        for i in range(3):
            input_array.append(bool(k & 1))
            k = k >> 1
        output_states = network.propagate(input_array)
        if output_states == desired_output:
            correct_count += 1
        if report:
            print(f"Input: {input_array}, Desired: {desired_output}, Output: {output_states} - {('correct' if output_states == desired_output else 'wrong')}")
    if report:
        print(f"Number of correct outputs out of 8: {correct_count}")
    return network

# Run the test

np.random.seed(42167319)
layers = [3, 9, 2]
network = train_and_evaluate(layers, 0.01, 2000, True)


