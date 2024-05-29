import numpy as np

# Global debug variable
debug = 0

# Set the random seed for reproducibility
np.random.seed(42)

class Synapse:
    def __init__(self, neuron, is_excitatory=True):
        self.weight = np.random.rand()  # Initialize with a random weight
        self.neuron = neuron
        self.is_excitatory = is_excitatory

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

        # Calculate output layer deltas
        for i, neuron in enumerate(self.neurons[-1]):
            error = (1.0 if desired_output[i] else 0.0) - neuron.output
            neuron.delta = error  # Remove sigmoid derivative
            if debug == 1 or (debug > 1 and epoch is not None and epoch % debug == 0):
                print(f"Output neuron {i}: error = {error}, delta = {neuron.delta}")

        # Backpropagate the error to hidden layers
        for l in range(len(self.layers) - 2, -1, -1):
            for i, neuron in enumerate(self.neurons[l]):
                neuron.delta = sum(
                    [(synapse.weight if synapse.is_excitatory else -synapse.weight) * synapse.neuron.delta for synapse in neuron.synapses]
                )  # Remove sigmoid derivative
                if debug == 1 or (debug > 1 and epoch is not None and epoch % debug == 0):
                    layer_type = "Input neuron" if l == 0 else "Hidden neuron"
                    print(f"{layer_type} layer {l} neuron {i}: delta = {neuron.delta}")

        # Update the weights and potentially set the synapse type
        learning_rate = 0.1
        for l in range(len(self.layers) - 1):
            for neuron in self.neurons[l]:
                for synapse in neuron.synapses:
                    weight_change = learning_rate * neuron.output * synapse.neuron.delta
                    synapse.weight += weight_change
                    if debug == 1 or (debug > 1 and epoch is not None and epoch % debug == 0):
                        print(f"Synapse from neuron (layer {neuron.layer_index}, index {neuron.neuron_index}) to neuron (layer {synapse.neuron.layer_index}, index {synapse.neuron.neuron_index}): weight change = {weight_change}, new weight = {synapse.weight}")
                    # Set the type if necessary
                    if synapse.weight < 0:
                        synapse.set_type(False)  # Set to inhibitory
                        synapse.weight = abs(synapse.weight)
                    else:
                        synapse.set_type(True)  # Set to excitatory

# Example usage
network = Network([7, 20, 5])

# Input array of booleans
input_array = [True, False, True, False, False, True, False]
desired_output = [False, True, True, False, True]

# Train the network over multiple epochs
for epoch in range(1000):
    output_states = network.propagate(input_array, epoch)
    if debug == 1 or (debug > 1 and epoch % debug == 0):
        print(f"Epoch {epoch + 1}: Output states: {output_states}")
    network.backpropagate(desired_output, epoch)

# Final output after training
final_output = network.propagate(input_array)
print(f"Final Output states: {final_output}")
