#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "opticalComponent.h"
#include "opticalSurface.h"

#include "opticalSurfaces/PlanarCircle.h"
#include "opticalComponents/Mirror.h"
#include "opticalComponents/Lens.h"
#include "utils/readRaysFromFile.h"

#include "../thirdParty/eigen-3.4.0/Eigen/Dense"
#include "../utils/vec3.h"
#include "ray.h"  

using T = double;
using U = float;

template <typename T>
std::string vecToString(const vec3<T>& vec) {
    return std::to_string(vec(0)) + " " +
           std::to_string(vec(1)) + " " +
           std::to_string(vec(2));
}

int main() {

    std::vector<std::string> logs;

    auto rays = readRaysFromFile("utils/ray_input.dat");

    vec3<T> position(1.0, 1.0, 1.0);
    vec3<T> normal(0.0, 0.0, 1.0); 
    T diameter = 2.0; 

    std::unique_ptr<PlanarSurface<T, U>> circle = std::make_unique<PlanarCircle<T, U>>(position, normal, diameter);
    circle->generatePoints();

    std::unique_ptr<OpticalComponent<T, U>> lens = std::make_unique<ThinLens<T, U>>(std::move(circle), -0.5);

    std::ofstream outFile("rays.dat");

    if (!outFile) {
        std::cerr << "Failed to open components.dat for writing." << std::endl;
        return 0;
    }

    for (auto& ray : *rays) { 
        outFile << ray.position(0) << " " << ray.position(1) << " " << ray.position(2) << std::endl;

        auto [doesIntersect, intersectionPoint] = lens->getSurface()->intersects(ray);
        outFile << intersectionPoint(0) << " " << intersectionPoint(1) << " " << intersectionPoint(2) << std::endl;
        if (doesIntersect) {
            lens->handleLight(ray, intersectionPoint);
            outFile << ray.direction(0) << " " << ray.direction(1) << " " << ray.direction(2) << std::endl;

        } 
        outFile << "\n\n" << std::endl;
    }

    outFile.close();

    return 0;
}