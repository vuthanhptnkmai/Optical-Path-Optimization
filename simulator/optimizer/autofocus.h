#pragma once

#include "../simulator.h"
#include "sharpness.h"

template<typename T, typename U>
void autoFocus(std::unique_ptr<std::vector<Ray<double, float>>>& rays, 
            std::vector<std::unique_ptr<OpticalComponent<T, U>>>& components, 
            ThinLens<T, U>* lensPtr, Detector<T, U>* detectorPtr, T startZ, T endZ, T stepZ, bool verbose = false) {

    T bestZ = startZ;
    U bestSharpness = -std::numeric_limits<U>::max();

    for (T z = startZ; z <= endZ; z += stepZ) {
         // Create a copy of the rays for this iteration
        auto raysCopy = std::make_unique<std::vector<Ray<T, U>>>(*rays);

        // Adjust the lens position
        vec3<T> newPos = lensPtr->getSurfacePtr()->getPosition();
        newPos(2) = z;
        lensPtr->getSurfacePtr()->setPosition(newPos);

        if (verbose) {
            std::cout << "Adjusting lens to Z: " << z << std::endl;
        }

        // Simulate the optical path
        rayTracing(raysCopy, components);

        // Evaluate image sharpness
        U currentSharpness = sharpnessVariance(detectorPtr->getPixelGrid());
        if (verbose) {
            std::cout << "Current sharpness at Z " << z << ": " << currentSharpness << std::endl;
        }

        if (currentSharpness > bestSharpness) {
            bestSharpness = currentSharpness;
            bestZ = z;
        }

        // Reset the detector for the next iteration
        detectorPtr->resetPixelGrid();
    }

    // Set lens to the best focus position
    vec3<T> bestPos = lensPtr->getSurfacePtr()->getPosition();
    bestPos(2) = bestZ;
    lensPtr->getSurfacePtr()->setPosition(bestPos);

    std::cout << "Autofocus completed. Best focus position: " << bestZ << std::endl;

    return;
}
