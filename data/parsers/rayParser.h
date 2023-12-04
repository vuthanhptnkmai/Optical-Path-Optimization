#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>

#include "../../simulator/ray.h"
#include "../../utils/vec3.h"

/**
 * Sample ray data for the Ray class.
 * 
 * Each line of data represents the properties of a single ray in the system. 
 * The format for each ray is as follows:
 *   [X Y Z DX DY DZ WL I]
 * 
 * Where:
 *  - X, Y, Z: The starting position of the ray in 3D space. 
 *             These values represent the coordinates of the ray's initial point.
 * 
 *  - DX, DY, DZ: Components of the ray's direction vector. 
 *                This is a 3D vector indicating the direction in which the ray is traveling.
 *                These values will be normalized to ensure the direction vector is a unit vector.
 * 
 *  - WL: Wavelength of the ray, in nanometers (nm). 
 *        This value represents the color or energy level of the ray.
 * 
 *  - I: Intensity of the ray. 
 *       This is a measure of the strength or amplitude of the ray.
 * 
 * Example:
 *   0.2 3.0 0.0 -0.29294225063524 -2.1316032563673564 -0.3389379428831442 480 1
 *   This represents a ray starting at position (0.2, 3.0, 0.0), moving in the direction
 *   given by the vector (-0.29294225063524, -2.1316032563673564, -0.3389379428831442),
 *   with a wavelength of 480 nm and an intensity of 1.
 */

/**
 * Reads ray data from a file and creates a vector of Ray objects.
 * 
 * @param filename The name of the file containing ray data.
 * @return A unique pointer to a vector of Ray objects.
 */
template<typename T, typename U>
std::unique_ptr<std::vector<Ray<T, U>>> parseRaysDat(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return nullptr;
    }

    auto rays = std::make_unique<std::vector<Ray<T, U>>>();

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        
        vec3<T> position, direction;
        U wavelength, intensity;

        if (!(iss >> position(0) >> position(1) >> position(2) >> direction(0) >> direction(1) >> direction(2) >> wavelength >> intensity)) {
            std::cerr << "Error reading a line from the file!" << std::endl;
            continue;
        }

        rays->emplace_back(position, direction, wavelength, intensity);
    }

    return rays;
}