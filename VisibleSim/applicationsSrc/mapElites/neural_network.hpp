#ifndef NEURAL_NETWORK_HPP
#define NEURAL_NETWORK_HPP

#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib> // for rand and srand
#include <ctime> // for time

using namespace std;

// TODO: if needed add some biases

class NeuralNetwork {
    private:
        int numInputs;
        // for the moment we suppose that every hidden layer has the same number of neurons and the same activation function
        int numHiddenLayers;
        int numNeuronsPerHiddenLayer;
        int numOutputs;
        bool useBias; // by convention the bias is always the last element of the weights vectors
        vector<vector<vector<double>>> weights;
        vector<double> inputs;
        vector<double> outputs;
        function<double(double)> activationFunction;
        function<double(double)> activationFunctionOutput;
        
        double sigmoid(double x);
        double relu(double x);
        double tanh(double x);
        
        public:
            NeuralNetwork(int numInputs, int numHiddenLayers, int numNeuronsPerHiddenLayer, int numOutputs, bool useBias = false);
            void setWeights(vector<vector<vector<double>>> weights);
            void checkWeights(); // send an error if the weights are not of the right size
            void setActivationFunction(string activationFunction); // either "sigmoid", "relu" or "tanh" else default to sigmoid
            void setActivationFunctionOutput(string activationFunction); // either "sigmoid", "relu" or "tanh" else default to sigmoid
            vector<double> feedForward(vector<double> inputs); // feed forward the inputs through the network
            vector<vector<vector<double>>> generateRandomWeights(); // generate random weights for the network
            vector<vector<vector<double>>> reshapeWeights(vector<double> flatWeights); // reshape the flat weights into the weights of the network

};

#endif