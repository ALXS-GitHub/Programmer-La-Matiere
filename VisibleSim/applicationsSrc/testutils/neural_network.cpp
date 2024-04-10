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
 * @param numNeuronsPerHiddenLayer the number of neurons per hidden layer (do not take into account the bias)
 * @param numOutputs the number of outputs
 * @param useBias [defaults to false] whether to use a bias or not (by convention the bias is always the last element of the weights vectors)
*/
NeuralNetwork::NeuralNetwork(int numInputs, int numHiddenLayers, int numNeuronsPerHiddenLayer, int numOutputs, bool useBias) {
    this->numInputs = numInputs;
    this->numHiddenLayers = numHiddenLayers;
    this->numNeuronsPerHiddenLayer = numNeuronsPerHiddenLayer;
    this->numOutputs = numOutputs;
    this->activationFunction = bind(&NeuralNetwork::sigmoid, this, placeholders::_1); // default activation function is sigmoid
    this->activationFunctionOutput = bind(&NeuralNetwork::sigmoid, this, placeholders::_1); // default activation function is sigmoid
    this->useBias = useBias; // by convention the bias is always the last element of the weights vectors
}

/**
 * @brief Set the Weights object
 * 
 * @param weights the weights of the neural network
*/
void NeuralNetwork::setWeights(vector<vector<vector<double>>> weights) {

    // ! error handling, check if weights are of the right size
    if (weights.size() != static_cast<size_t>(numHiddenLayers + 1)) {
        throw runtime_error("\033[1;31m[WEIGHTS]\033[0m Invalid number of weight layers, expected " + to_string(numHiddenLayers + 1) + " got " + to_string(weights.size()));
    }

    for (int i = 0; i < numHiddenLayers + 1; i++) {
        if (weights[i].size() != static_cast<size_t>((i == numHiddenLayers ? numOutputs : numNeuronsPerHiddenLayer))) {
            throw runtime_error("\033[1;31m[WEIGHTS]\033[0m Invalid number of neurons in hidden layer " + to_string(i) + " expected " + to_string(i == numHiddenLayers ? numOutputs : numNeuronsPerHiddenLayer) + " got " + to_string(weights[i].size()));
        }
        for (int j = 0; j < (i == numHiddenLayers ? numOutputs : numNeuronsPerHiddenLayer); j++) {
            if (weights[i][j].size() != static_cast<size_t>((i == 0 ? numInputs : numNeuronsPerHiddenLayer) + (useBias ? 1 : 0))) {
                throw runtime_error("\033[1;31m[WEIGHTS]\033[0m Invalid number of inputs in hidden layer " + to_string(i) + " neuron " + to_string(j) + " expected " + to_string((i == 0 ? numInputs : numNeuronsPerHiddenLayer) + (useBias ? 1 : 0)) + " got " + to_string(weights[i][j].size()));
            }
        }
    }

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
 * @brief Set the Activation Function Output object
 * 
 * @param activationFunction the activation function to use
*/
void NeuralNetwork::setActivationFunctionOutput(string activationFunction) {
    if (activationFunction == "sigmoid") {
        this->activationFunctionOutput = bind(&NeuralNetwork::sigmoid, this, placeholders::_1);
    } else if (activationFunction == "relu") {
        this->activationFunctionOutput = bind(&NeuralNetwork::relu, this, placeholders::_1);
    } else if (activationFunction == "tanh") {
        this->activationFunctionOutput = bind(&NeuralNetwork::tanh, this, placeholders::_1);
    } else {
        cout << "Invalid activation function, defaulting to sigmoid" << endl;
        this->activationFunctionOutput = bind(&NeuralNetwork::sigmoid, this, placeholders::_1);
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

            if (useBias) { // adding the bias
                sum += weights[i][j][currentNumInputs];
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

        if (useBias) { // adding the bias
            sum += weights[numHiddenLayers][i][currentNumInputs];
        }

        outputs[i] = this->activationFunctionOutput(sum); // $ activation function
    }

    return this->outputs;
}

