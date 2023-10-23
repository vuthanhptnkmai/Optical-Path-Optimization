#include <iostream>
#include "utils/vec3.h"
#include "simulator/photon.h"
#include "data/photonParser.h"

int main() {
    // Create a photon instance
    vec3<double> initialPosition(1.0, 2.0, 3.0);
    vec3<double> initialDirection(0.5, 0.5, 0.5);
    double initialWavelength = 500.0;  // in nanometers, for example
    double initialIntensity = 1.0;     // arbitrary unit

    Photon<double, double> photon(initialPosition, initialDirection, initialWavelength, initialIntensity);

    // Print out the photon's properties
    std::cout << "Photon's Position: [" << photon.position[0] << ", " << photon.position[1] << ", " << photon.position[2] << "]" << std::endl;
    std::cout << "Photon's Direction: [" << photon.direction[0] << ", " << photon.direction[1] << ", " << photon.direction[2] << "]" << std::endl;
    std::cout << "Photon's Wavelength: " << photon.wavelength << " nm" << std::endl;
    std::cout << "Photon's Intensity: " << photon.intensity << std::endl;

    return 0;
}

