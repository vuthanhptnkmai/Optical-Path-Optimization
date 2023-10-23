#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "opticalComponent.h"
#include "opticalSurface.h"

#include "opticalSurfaces/PlanarCircle.h"
#include "opticalComponents/Mirror.h"
#include "opticalComponents/lens.h"
#include "utils/readRaysFromFile.h"

#include "../thirdParty/eigen-3.4.0/Eigen/Dense"
#include "../utils/vec3.h"
#include "ray.h"  

using T = double;
using U = float;

int main() {

    std::vector<std::string> logs;

    auto rays = readRaysFromFile("utils/ray_input.dat");

    vec3<T> position(1.0, 1.0, 1.0);
    vec3<T> normal(0.0, 0.0, 1.0); 
    T diameter = 2.0; 
    std::unique_ptr<OpticalSurface<T, U>> circle = std::make_unique<PlanarCircle<T, U>>(position, normal, diameter);
    std::unique_ptr<OpticalComponent<T, U>> mirror = std::make_unique<PlanarMirror<T, U>>(std::move(circle));
	circle->generatePoints("component1.dat");
	
    vec3<T> position1(0.0, 1.0, 0.0);
    vec3<T> normal1(0.0, 1.0, 0.0); 
    T diameter1 = 1.0; 
    std::unique_ptr<PlanarSurface<T, U>> circle1 = std::make_unique<PlanarCircle<T, U>>(position1, normal1, diameter1);
    std::unique_ptr<OpticalComponent<T, U>> lens = std::make_unique<ThinLens<T, U>>(std::move(circle1), 0.5);
    circle1->generatePoints("component2.dat");

    std::ofstream outFile("rays.dat");

    if (!outFile) {
        std::cerr << "Failed to open components.dat for writing." << std::endl;
        return 0;
    }

    for (auto& ray : *rays) { 
        outFile << ray.position(0) << " " << ray.position(1) << " " << ray.position(2) << std::endl;

        auto [doesIntersect, intersectionPoint] = mirror->getSurface()->intersects(ray);
        outFile << intersectionPoint(0) << " " << intersectionPoint(1) << " " << intersectionPoint(2) << std::endl;
        if (doesIntersect) {
            mirror->handleLight(ray, intersectionPoint);
            outFile << ray.direction(0) << " " << ray.direction(1) << " " << ray.direction(2) << std::endl;
			} else {
			outFile << "\n\n" << std::endl; 
            continue;
		}

        auto [doesIntersect1, intersectionPoint1] = lens->getSurface()->intersects(ray);
        outFile << intersectionPoint1(0) << " " << intersectionPoint1(1) << " " << intersectionPoint1(2) << std::endl;
        if (doesIntersect1) {
            lens->handleLight(ray, intersectionPoint1);
            outFile << ray.direction(0) << " " << ray.direction(1) << " " << ray.direction(2) << std::endl;
		} else {
			outFile << "\n\n" << std::endl; 
            continue;
		}
    }

    outFile.close();

    return 0;
}