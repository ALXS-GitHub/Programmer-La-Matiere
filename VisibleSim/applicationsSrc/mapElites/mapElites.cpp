
/**
 * @file mapElites.cpp
 * Generated by VisibleSim BlockCode Generator
 * https://services-stgi.pu-pm.univ-fcomte.fr/visiblesim/generator.php#
 * @author yourName
 * @date 2024-01-27                                                                     
 **/
 
#include <iostream>
#include "mapElitesCode.hpp"

using namespace std;
using namespace Catoms3D;

int main(int argc, char **argv) {
    try {
        createSimulator(argc, argv, mapElitesCode::buildNewBlockCode);
        getSimulator()->printInfo();
        BaseSimulator::getWorld()->printInfo();
        deleteSimulator();
    } catch(std::exception const& e) {
        cerr << "Uncaught exception: " << e.what();
    }

    return 0;
}