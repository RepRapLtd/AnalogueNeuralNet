import numpy as np

# Global debug variable
debug = 0

# Set the random seed for reproducibility
np.random.seed(42)

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
        if debug > 0:
            print(f"Neuron {self.layer_index}-{self.neuron_index} updated synapse count. Excitatory: {self.excitatory_count}, Inhibitory: {self.inhibitory_count}")

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
        if self.output:
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

    def backpropagate(self, desired_output, learning_rate = 0.01, epoch=None):
        if len(desired_output) != len(self.neurons[-1]):
            raise ValueError("Desired output length must match the number of output neurons.")

        # Calculate output layer deltas
        for i, neuron in enumerate(self.neurons[-1]):
            error = (1.0 if desired_output[i] else 0.0) - (1.0 if neuron.output else 0.0)
            neuron.delta = error  # Remove sigmoid derivative
            if debug == 1 or (debug > 1 and epoch is not None and epoch % debug == 0):
                print(f"Output neuron {i}: error = {error}, delta = {neuron.delta}")

        # Backpropagate the error to hidden layers

        for l in range(len(self.layers) - 2, -1, -1):
            for i, neuron in enumerate(self.neurons[l]):
                neuron.delta = 0.0
                for synapse in neuron.synapses:
                    if synapse.neuron.excitatory_count > 0:
                        plus = synapse.weight/synapse.neuron.excitatory_count
                    else:
                        plus = 0.0
                    if synapse.neuron.inhibitory_count > 0:
                        minus = -synapse.weight/synapse.neuron.inhibitory_count
                    else:
                        minus = 0.0
                    neuron.delta += (plus if synapse.is_excitatory else minus) * synapse.neuron.delta
                if debug == 1 or (debug > 1 and epoch is not None and epoch % debug == 0):
                    layer_type = "Input neuron" if l == 0 else "Hidden neuron"
                    print(f"{layer_type} layer {l} neuron {i}: delta = {neuron.delta}")

        # Update the weights and potentially set the synapse type
        max_weight = float('-inf')
        for l in range(len(self.layers) - 1):
            for neuron in self.neurons[l]:
                for synapse in neuron.synapses:
                    weight_change = learning_rate * (1.0 if neuron.output else 0.0) * synapse.neuron.delta
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
                synapse.set_weight(synapse.weight * scale_factor)
                if debug == 1 or (debug > 1 and epoch is not None and epoch % debug == 0):
                    print(f"Rescaled synapse weight: {synapse.weight}")

    def network_to_string(self):
        result = ""
        for layer_index, layer in enumerate(self.neurons):
            result += f"Layer {layer_index}:\n"
            for neuron in layer:
                result += f"  Neuron {neuron.neuron_index}:\n"
                for synapse in neuron.synapses:
                    result += f"    -> Neuron {synapse.neuron.layer_index}-{synapse.neuron.neuron_index} (Weight: {synapse.weight:.4f}, {'Excitatory' if synapse.is_excitatory else 'Inhibitory'})\n"
        return result


np.random.seed(42)

network = Network([3, 4, 1])
print(network.network_to_string())

for epoch in range(50):
    for i in range(4):
        input_array = [not not i & 1, not not (i >> 1) & 1, True]
        desired_output = [i != 3]
        network.propagate(input_array)
        network.backpropagate(desired_output, learning_rate = 0.1)

print(network.network_to_string())

for i in range(4):
    input_array = [not not i & 1, not not (i >> 1) & 1, True]
    desired_output = [i != 3]
    output = network.propagate(input_array)
    print(f"input: {input_array}, desired: {desired_output} gives {output[0]}")



'''

# Function to train and evaluate the network
def train_and_evaluate(hidden_layer_size, lr, epochs):
    # Set the random seed for reproducibility
    np.random.seed(42)

    # Generate random training data
    training_data = {i: [bool(int(x)) for x in f'{np.random.randint(0, 4):02b}'] for i in range(8)}

    # Initialize the network
    network = Network([3, hidden_layer_size, 2])
    print(network.network_to_string())

    # Train the network on the training data
    for epoch in range(epochs):
        for input_value, desired_output in training_data.items():
            input_array = [bool(int(x)) for x in f'{input_value:03b}']
            network.propagate(input_array)
            network.backpropagate(desired_output, learning_rate = lr)

    print(network.network_to_string())

    # Evaluate the network on the training data
    correct_count = 0
    for input_value, desired_output in training_data.items():
        input_array = [bool(int(x)) for x in f'{input_value:03b}']
        output_states = network.propagate(input_array)
        if output_states == desired_output:
            correct_count += 1
        print(f"Input: {input_array}, Desired: {desired_output}, Output: {output_states}, Correct: {output_states == desired_output}")

    print(f"Number of correct outputs out of 8: {correct_count}")
    return correct_count

# Define parameter ranges
hidden_layer_sizes = [5]#, 10, 20]
learning_rates = [0.001]#, 0.01, 0.1]
epoch_counts = [1000]#, 5000, 10000]

# Test different combinations of parameters
best_correct_count = 0
best_params = None
for hls in hidden_layer_sizes:
    for lr in learning_rates:
        for ec in epoch_counts:
            print(f"\nTesting with hidden_layer_size={hls}, learning_rate={lr}, epochs={ec}")
            correct_count = train_and_evaluate(hls, lr, ec)
            print(f"correct={correct_count}")
            if correct_count > best_correct_count:
                best_correct_count = correct_count
                best_params = (hls, lr, ec)

print(f"\nBest performance: {best_correct_count} correct outputs with hidden_layer_size={best_params[0]}, learning_rate={best_params[1]}, epochs={best_params[2]}")



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
'''
