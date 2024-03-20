#include <iostream>
#include "../src/neural_network.hpp"

int main() {
    cout << "Neural Network Test\n";
    // Create a neural network with 2 inputs, 2 hidden layers, 2 neurons per hidden layer, and 1 output
    NeuralNetwork nn = NeuralNetwork(2, 2, 2, 1);

    // Set the weights for the neural network
    vector<vector<vector<double>>> weights = {
        // Weights for first hidden layer
        {
            {0.1, 0.2}, // Weights for first neuron
            {0.3, 0.4}  // Weights for second neuron
        },
        // Weights for second hidden layer
        {
            {0.5, 0.6}, // Weights for first neuron
            {0.7, 0.8}  // Weights for second neuron
        },
        // Weights for output layer
        {
            {0.9, 1.0}  // Weights for output neuron
        }
    };
    nn.setWeights(weights);

    // Feed forward some inputs through the neural network
    vector<double> inputs = {1.0, 2.0};
    vector<double> outputs = nn.feedForward(inputs);

    // Print the outputs
    cout << "Outputs: ";
    for (double output : outputs) {
        cout << output << " ";
    }
    cout << endl;

    return 0;
}