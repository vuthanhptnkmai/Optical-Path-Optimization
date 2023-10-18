#include <iostream>
#include "Ray.h"  // Assuming the Ray class is defined in Ray.h

// Function to test the Ray class
void testRay() {
    // Create a Ray instance
    Ray<double, double> ray(vec3<double>(0, 0, 0), vec3<double>(1, 1, 1), 650.0, 1.0);

    // Test getters
    std::cout << "Position: " << ray.getPosition().transpose() << std::endl;
    std::cout << "Direction: " << ray.getDirection().transpose() << std::endl;
    std::cout << "Wavelength: " << ray.getWavelength() << std::endl;
    std::cout << "Intensity: " << ray.getIntensity() << std::endl;

    // Test setters
    ray.setPosition(vec3<double>(1, 1, 1));
    ray.setDirection(vec3<double>(0, 0, 1));
    ray.setWavelength(550.0);
    ray.setIntensity(0.8);

    std::cout << "\nAfter setting new values:\n";
    std::cout << "Position: " << ray.getPosition().transpose() << std::endl;
    std::cout << "Direction: " << ray.getDirection().transpose() << std::endl;
    std::cout << "Wavelength: " << ray.getWavelength() << std::endl;
    std::cout << "Intensity: " << ray.getIntensity() << std::endl;
}

int main() {
    testRay();
    return 0;
}