#include <fstream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>

#include "../../utils/vec3.h"
#include "../ray.h"


std::unique_ptr<std::vector<Ray<double, float>>> readRaysFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return nullptr;
    }

    auto rays = std::make_unique<std::vector<Ray<double, float>>>();

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        
        vec3<double> position, direction;
        int wavelength, intensity;

        if (!(iss >> position(0) >> position(1) >> position(2) >> direction(0) >> direction(1) >> direction(2) >> wavelength >> intensity)) {
            std::cerr << "Error reading a line from the file!" << std::endl;
            continue;
        }

        rays->emplace_back(position, direction, wavelength, intensity);
    }

    return rays;
}