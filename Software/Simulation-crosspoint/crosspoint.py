import numpy as np

# Global debug variable
debug = 0

# Set the random seed for reproducibility
np.random.seed(42)

class Synapse:
    def __init__(self, neuron):
        self.weight = np.random.uniform(0.0, 1.0)  # Initialize weights between 0 and 1
        self.is_excitatory = np.random.choice([True, False])  # Randomly set excitatory or inhibitory
        self.neuron = neuron

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
        self.is_excitatory = is_excitatory

class Neuron:
    def __init__(self, layer_index, neuron_index):
        self.layer_index = layer_index
        self.neuron_index = neuron_index
        self.synapses = []
        self.excitations = 0.0
        self.inhibitions = 0.0
        self.output = 0.0  # Output value after activation
        self.delta = 0.0  # Used for backpropagation

    def add_synapse(self, synapse):
        self.synapses.append(synapse)

    def receive_excitation(self, amount):
        self.excitations += amount

    def receive_inhibition(self, amount):
        self.inhibitions += amount

    def check_fire(self):
        self.output = 1.0 if self.excitations > self.inhibitions else 0.0
        if self.output == 1.0:
            self.fire()

    def fire(self):
        for synapse in self.synapses:
            synapse.transmit()

    def reset(self):
        self.excitations = 0.0
        self.inhibitions = 0.0

class Network:
    def __init__(self, layers):
        self.layers = layers
        self.neurons = []
        self.synapses = []
        self.create_network()

    def create_network(self):
        # Create neurons layer by layer
        for layer_index, layer_size in enumerate(self.layers):
            layer = [Neuron(layer_index, neuron_index) for neuron_index in range(layer_size)]
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

    def reset_network(self):
        for layer in self.neurons:
            for neuron in layer:
                neuron.reset()

    def check_fire_all(self):
        for layer in self.neurons:
            for neuron in layer:
                neuron.check_fire()

    def propagate(self, input_array, epoch=None):
        if len(input_array) != len(self.neurons[0]):
            raise ValueError("Input array length must match the number of input neurons.")

        # Reset the network before propagation
        self.reset_network()

        # Fire the input neurons corresponding to TRUE in the input array
        for i, state in enumerate(input_array):
            if state:
                self.neurons[0][i].receive_excitation(1.0)  # Assuming maximum excitation

        # Check if all neurons should fire
        self.check_fire_all()

        # Get the state of the output layer
        output_layer = self.neurons[-1]
        output_states = [neuron.output for neuron in output_layer]

        return output_states

    def backpropagate(self, desired_output, epoch=None):
        if len(desired_output) != len(self.neurons[-1]):
            raise ValueError("Desired output length must match the number of output neurons.")

        max_weight = float('-inf')

        # Calculate output layer deltas
        for i, neuron in enumerate(self.neurons[-1]):
            error = (1.0 if desired_output[i] else 0.0) - neuron.output
            neuron.delta = error
            if debug == 1 or (debug > 1 and epoch is not None and epoch % debug == 0):
                print(f"Output neuron {i}: error = {error}, delta = {neuron.delta}")

        # Backpropagate the error to hidden layers
        for l in range(len(self.layers) - 2, -1, -1):
            for i, neuron in enumerate(self.neurons[l]):
                neuron.delta = sum(
                    [(synapse.weight if synapse.is_excitatory else -synapse.weight) * synapse.neuron.delta for synapse in neuron.synapses]
                )
                if debug == 1 or (debug > 1 and epoch is not None and epoch % debug == 0):
                    layer_type = "Input neuron" if l == 0 else "Hidden neuron"
                    print(f"{layer_type} layer {l} neuron {i}: delta = {neuron.delta}")

        # Update the weights and track the maximum absolute weight
        learning_rate = 0.01  # Adjusted learning rate for stability
        for l in range(len(self.layers) - 1):
            for neuron in self.neurons[l]:
                for synapse in neuron.synapses:
                    weight_change = learning_rate * neuron.output * synapse.neuron.delta
                    synapse.weight += weight_change
                    abs_weight = abs(synapse.weight)
                    max_weight = max(max_weight, abs_weight)
                    if debug == 1 or (debug > 1 and epoch is not None and epoch % debug == 0):
                        print(f"Synapse from neuron (layer {neuron.layer_index}, index {neuron.neuron_index}) to neuron (layer {synapse.neuron.layer_index}, index {synapse.neuron.neuron_index}): weight change = {weight_change}, new weight = {synapse.weight}")
                    # Set the type if necessary
                    if synapse.weight < 0:
                        synapse.set_type(False)  # Set to inhibitory
                        synapse.weight = abs_weight
                    else:
                        synapse.set_type(True)  # Set to excitatory

        # Rescale weights to lie between 0 and 1
        scale_factor = 1.0 / max_weight
        for layer in self.synapses:
            for synapse in layer:
                synapse.weight *= scale_factor
                if debug == 1 or (debug > 1 and epoch is not None and epoch % debug == 0):
                    print(f"Rescaled synapse weight: {synapse.weight}")

# Example usage

correct_count = 0

# Train and evaluate the network for each desired output pattern
for desired_output_pattern in range(32):
    network = Network([7, 20, 5])  # Reinitialize the network for each pattern
    input_array = [True, False, True, False, False, True, False]
    desired_output = [bool(int(x)) for x in f'{desired_output_pattern:05b}']

    # Train the network on the current desired output pattern
    for epoch in range(100):
        network.propagate(input_array, epoch)
        network.backpropagate(desired_output, epoch)

    # Evaluate the network on the current desired output pattern
    output_states = network.propagate(input_array)
    if output_states == desired_output:
        correct_count += 1
    print(f"Desired: {desired_output}, Output: {output_states}, Correct: {output_states == desired_output}")

print(f"Number of correct outputs out of 32: {correct_count}")
