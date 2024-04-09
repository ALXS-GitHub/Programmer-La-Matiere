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

    // test with biases

    NeuralNetwork nn2 = NeuralNetwork(2, 2, 2, 1, true);

    vector<vector<vector<double>>> weights2 = {
        // Weights for first hidden layer
        {
            {0.1, 0.2, 0}, // Weights for first neuron
            {0.3, 0.4, 0}  // Weights for second neuron
        },
        // Weights for second hidden layer
        {
            {0.5, 0.6, 0}, // Weights for first neuron
            {0.7, 0.8, 0}  // Weights for second neuron
        },
        // Weights for output layer
        {
            {0.9, 1.0, 0}  // Weights for output neuron
        }
    };

    nn2.setWeights(weights2);

    vector<double> inputs2 = {1.0, 2.0};
    vector<double> outputs2 = nn2.feedForward(inputs2);

    cout << "Outputs with null biases: ";
    for (double output : outputs2) {
        cout << output << " ";
    }
    cout << endl;

    NeuralNetwork nn3 = NeuralNetwork(2, 2, 2, 1, true);

    vector<vector<vector<double>>> weights3 = {
        // Weights for first hidden layer
        {
            {0.1, 0.2, 0.5}, // Weights for first neuron
            {0.3, 0.4, 0.5}  // Weights for second neuron
        },
        // Weights for second hidden layer
        {
            {0.5, 0.6, 0.5}, // Weights for first neuron
            {0.7, 0.8, 0.5}  // Weights for second neuron
        },
        // Weights for output layer
        {
            {0.9, 1.0, 0.5}  // Weights for output neuron
        }
    };

    nn3.setWeights(weights3);

    vector<double> inputs3 = {1.0, 2.0};
    vector<double> outputs3 = nn3.feedForward(inputs3);

    cout << "Outputs with non null biases: ";
    for (double output : outputs3) {
        cout << output << " ";
    }
    cout << endl;

    return 0;
}