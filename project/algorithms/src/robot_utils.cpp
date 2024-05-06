#include "robot_utils.hpp"

uint64_t Utils::motionsProcessed = 0;
vector<vector<vector<double>>> Utils::weights;
bool Utils::weightsReceived = false;
unordered_set<vector<int>, Utils::CoordinatesHash> Utils::takenDestinations;

void Utils::addTakenDestination(vector<int> destination) {
    takenDestinations.insert(destination);
}

void Utils::removeTakenDestination(vector<int> destination) {
    takenDestinations.erase(destination);
}

bool Utils::isDestinationTaken(vector<int> destination) {
    return takenDestinations.find(destination) != takenDestinations.end();
}

void Utils::printTakenDestinations() {

    if (takenDestinations.empty()) {
        cout << "No taken destinations" << endl;
        return;
    }

    for (auto &dest : takenDestinations) {
        cout << "Destination : " << dest[0] << ", " << dest[1] << ", " << dest[2] << endl;
    }
}

int Utils::getCubeDistance(vector<int> pos1, vector<int> pos2) {
    return max(abs(pos1[0] - pos2[0]), max(abs(pos1[1] - pos2[1]), abs(pos1[2] - pos2[2])));
}

// getters and setters

uint64_t Utils::getMotionsProcessed() {
    return motionsProcessed;
}

void Utils::incrementMotionsProcessed() {
    motionsProcessed++;
}

bool Utils::areWeightsReceived() {
    return weightsReceived;
}

void Utils::setWeightsReceived(bool received) {
    weightsReceived = received;
}

vector<vector<vector<double>>>& Utils::getWeights() {
    return weights;
}

void Utils::setWeights(vector<vector<vector<double>>> w) {
    weights = w;
}