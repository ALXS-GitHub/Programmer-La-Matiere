#ifndef ROBOT_UTILS_HPP
#define ROBOT_UTILS_HPP

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_set>

using namespace std;

class Utils
{
    private:
        static uint64_t motionsProcessed;
        static bool weightsReceived;
        static vector<vector<vector<double>>> weights; // the weights of the neural network

    public:
        struct CoordinatesHash {
            size_t operator()(const std::vector<int>& v) const {
                std::hash<int> hasher;
                size_t seed = 0;
                for (int i : v) {
                    seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
                }
                return seed;
            }
        };
        static unordered_set<vector<int>, CoordinatesHash> takenDestinations; // list of the destinations already taken (to avoid having two modules going to the same destination)
        static void addTakenDestination(vector<int> destination);
        static void removeTakenDestination(vector<int> destination);
        static bool isDestinationTaken(vector<int> destination);
        static void printTakenDestinations(); // Especially for debugging
        static int getCubeDistance(vector<int> pos1, vector<int> pos2); // get the distance between two positions in the cube

        // getters and setters
        static uint64_t getMotionsProcessed();
        static void incrementMotionsProcessed();
        static bool areWeightsReceived();
        static void setWeightsReceived(bool received);
        static vector<vector<vector<double>>>& getWeights();
        static void setWeights(vector<vector<vector<double>>> w);

};

#endif // ROBOT_UTILS_HPP