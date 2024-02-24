/*!
 * @file replay.cpp
 * @brief Main file of the replayer
 * @author Mattéo Daluz
 */


#include <iostream>
#include "replay.hpp"

using namespace std;
using namespace Replay;

int main(int argc, char **argv) {
    cout << "Beginning of the program .." << endl;
    createPlayer(argc, argv);


    return 0;
}