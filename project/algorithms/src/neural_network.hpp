#include <iostream>
#include <vector>
#include <functional>

using namespace std;

// TODO: if needed add some biases

class NeuralNetwork {
    private:
        int numInputs;
        // for the moment we suppose that every hidden layer has the same number of neurons and the same activation function
        int numHiddenLayers;
        int numNeuronsPerHiddenLayer;
        int numOutputs;
        vector<vector<vector<double>>> weights;
        vector<double> inputs;
        vector<double> outputs;
        function<double(double)> activationFunction;
        
        double sigmoid(double x);
        double relu(double x);
        double tanh(double x);
        
        public:
            NeuralNetwork(int numInputs, int numHiddenLayers, int numNeuronsPerHiddenLayer, int numOutputs);
            void setWeights(vector<vector<vector<double>>> weights);
            void setActivationFunction(string activationFunction); // either "sigmoid", "relu" or "tanh" else default to sigmoid
            vector<double> feedForward(vector<double> inputs); // feed forward the inputs through the network

};