#include <iostream>
#include <cmath>

#include "neural_network.hpp"

using namespace std;

double NeuralNetwork::sigmoid(double x) {
    return 1 / (1 + exp(-x));
}

double NeuralNetwork::relu(double x) {
    return x > 0 ? x : 0;
}

double NeuralNetwork::tanh(double x) {
    return std::tanh(x); // std:: to avoid ambiguity between cmath and the NeuralNetwork class
}

/**
 * @brief Construct a new Neural Network
 * 
 * @param numInputs the number of inputs
 * @param numHiddenLayers the number of hidden layers
 * @param numNeuronsPerHiddenLayer the number of neurons per hidden layer
 * @param numOutputs the number of outputs
*/
NeuralNetwork::NeuralNetwork(int numInputs, int numHiddenLayers, int numNeuronsPerHiddenLayer, int numOutputs) {
    this->numInputs = numInputs;
    this->numHiddenLayers = numHiddenLayers;
    this->numNeuronsPerHiddenLayer = numNeuronsPerHiddenLayer;
    this->numOutputs = numOutputs;
    this->activationFunction = bind(&NeuralNetwork::sigmoid, this, placeholders::_1); // default activation function is sigmoid
}

/**
 * @brief Set the Weights object
 * 
 * @param weights the weights of the neural network
*/
void NeuralNetwork::setWeights(vector<vector<vector<double>>> weights) {
    this->weights = weights;
}


/**
 * @brief Set the Activation Function object
 * 
 * @param activationFunction the activation function to use
*/
void NeuralNetwork::setActivationFunction(string activationFunction) {
    if (activationFunction == "sigmoid") {
        this->activationFunction = bind(&NeuralNetwork::sigmoid, this, placeholders::_1);
    } else if (activationFunction == "relu") {
        this->activationFunction = bind(&NeuralNetwork::relu, this, placeholders::_1);
    } else if (activationFunction == "tanh") {
        this->activationFunction = bind(&NeuralNetwork::tanh, this, placeholders::_1);
    } else {
        cout << "Invalid activation function, defaulting to sigmoid" << endl;
        this->activationFunction = bind(&NeuralNetwork::sigmoid, this, placeholders::_1);
    }
}

/**
 * @brief Feed forward the inputs through the network
 * 
 * @param inputs the inputs to feed forward
 * @return vector<double> the outputs of the network
*/
vector<double> NeuralNetwork::feedForward(vector<double> inputs) {

    // ! error handling
    if (weights.empty()) {
        throw runtime_error("Weights not set");
    }

    this->inputs = inputs; // initial inputs
    this->outputs = vector<double>(numOutputs); // final outputs
    int currentNumInputs = numInputs; // only useful for the change between the input layer and the first hidden layer
    
    for (int i = 0; i < numHiddenLayers; i++) { // for each hidden layer
        vector<double> newInputs = vector<double>(numNeuronsPerHiddenLayer); // intermediate outputs
        for (int j = 0; j < numNeuronsPerHiddenLayer; j++) { // for each neuron in the current hidden layer
            double sum = 0;
            for (int k = 0; k < currentNumInputs; k++) { // sum of the products of the inputs and the weights
                sum += inputs[k] * weights[i][j][k]; 
            }
            newInputs[j] = this->activationFunction(sum); // $ activation function
        }
        inputs = newInputs;
        currentNumInputs = numNeuronsPerHiddenLayer; // setting the number of inputs for the next layer (for hidden layers)
    }

    // output layer
    for (int i = 0; i < numOutputs; i++) { // for each output neuron
        double sum = 0;
        for (int j = 0; j < currentNumInputs; j++) { // sum of the products of the inputs and the weights
            sum += inputs[j] * weights[numHiddenLayers][i][j];
        }
        outputs[i] = this->activationFunction(sum); // $ activation function
    }

    return this->outputs;
}

