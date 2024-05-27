import numpy as np

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

    def toggle_type(self):
        self.is_excitatory = not self.is_excitatory

class Neuron:
    def __init__(self):
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
        self.reset()

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
        for layer_size in self.layers:
            layer = [Neuron() for _ in range(layer_size)]
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

    def propagate(self, input_array):
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

    def backpropagate(self, desired_output):
        if len(desired_output) != len(self.neurons[-1]):
            raise ValueError("Desired output length must match the number of output neurons.")

        # Calculate output layer deltas
        for i, neuron in enumerate(self.neurons[-1]):
            error = (1.0 if desired_output[i] else 0.0) - neuron.output
            neuron.delta = error * self.sigmoid_derivative(neuron.excitations - neuron.inhibitions)
            print(f"Output neuron {i}: error = {error}, delta = {neuron.delta}")

        # Backpropagate the error to hidden layers
        for l in range(len(self.layers) - 2, -1, -1):
            for i, neuron in enumerate(self.neurons[l]):
                neuron.delta = sum([synapse.weight * synapse.neuron.delta for synapse in neuron.synapses]) * self.sigmoid_derivative(neuron.excitations - neuron.inhibitions)
                print(f"Hidden neuron layer {l} neuron {i}: delta = {neuron.delta}")

        # Update the weights and potentially toggle the synapse type
        learning_rate = 0.1
        for l in range(len(self.layers) - 1):
            for neuron in self.neurons[l]:
                for synapse in neuron.synapses:
                    weight_change = learning_rate * neuron.output * synapse.neuron.delta
                    synapse.weight += weight_change
                    print(f"Synapse from neuron {neuron} to neuron {synapse.neuron}: weight change = {weight_change}, new weight = {synapse.weight}")
                    # Toggle the type if necessary
                    if synapse.weight < 0:
                        synapse.toggle_type()
                        synapse.weight = abs(synapse.weight)
                        print(f"Synapse from neuron {neuron} to neuron {synapse.neuron} toggled to {'excitatory' if synapse.is_excitatory else 'inhibitory'}")

    def sigmoid_derivative(self, z):
        sigmoid = 1 / (1 + np.exp(-z))
        return sigmoid * (1 - sigmoid)

# Example usage
network = Network([7, 20, 5])

# Input array of booleans
input_array = [True, False, True, False, False, True, False]
desired_output = [False, True, True, False, True]

# Train the network over multiple epochs
for epoch in range(100):
    output_states = network.propagate(input_array)
    print(f"Epoch {epoch + 1}: Output states: {output_states}")
    network.backpropagate(desired_output)

# Final output after training
final_output = network.propagate(input_array)
print(f"Final Output states: {final_output}")
