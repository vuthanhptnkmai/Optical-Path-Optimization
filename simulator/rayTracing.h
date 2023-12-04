#pragma once 

#include "simulator.h"

template<typename T, typename U>
void rayTracing(std::unique_ptr<std::vector<Ray<double, float>>>& rays, std::vector<std::unique_ptr<OpticalComponent<T, U>>>& components) {
    
    ThinLens<T, U>* lensPtr = dynamic_cast<ThinLens<T, U>*>(components[1].get());
    Detector<T, U>* detectorPtr = dynamic_cast<Detector<T, U>*>(components.back().get());

    // #pragma omp parallel for
    for (auto& ray : *rays) {
        for (const auto& component : components) {
        // for (auto& component : components) {
            auto [doesIntersect, intersectionPoint] = component->getSurfacePtr()->intersects(ray);
            if (doesIntersect) {
                component->handleLight(ray, intersectionPoint);
            } else {
                break; // Skip remaining components if no intersection
            }
        }
    }
}

template<typename T, typename U>
void rayTracingWithLogging(std::unique_ptr<std::vector<Ray<T, U>>>& rays, 
                           std::vector<std::unique_ptr<OpticalComponent<T, U>>>& components) {
    std::ofstream raysFile("../visualization/rays.dat");
    if (!raysFile) {
        throw std::runtime_error("Failed to open rays.dat for writing.");
    }

    // Core ray tracing logic with logging...
    for (auto& ray : *rays) {
        // Log ray's initial position
        raysFile << ray.position(0) << " " << ray.position(1) << " " << ray.position(2) << std::endl;

        for (const auto& component : components) {
            //raysFile << ray.position(0) << " " << ray.position(1) << " " << ray.position(2) << std::endl;
            auto [doesIntersect, intersectionPoint] = component->getSurfacePtr()->intersects(ray);
            raysFile << intersectionPoint(0) << " " << intersectionPoint(1) << " " << intersectionPoint(2) << std::endl;
            
            if (doesIntersect) {
                component->handleLight(ray, intersectionPoint);
            } else {
                break; // Skip remaining components if no intersection
            }
        }

        // Log a separator between rays
        raysFile << "\n\n";
    }

    raysFile.close();
    std::cout << "Ray data generation completed successfully." << std::endl;

            // Open a file to write component data
        std::string componentsFileName = "../visualization/components.dat";
        std::ofstream componentsFile(componentsFileName, std::ios::out);
        if (!componentsFile) {
            throw std::runtime_error("Failed to open " + componentsFileName + " for writing.");
        }

        // Loop through the vector and generate points for each component's surface
        for (const auto& component : components) {
            auto surface = component->getSurfacePtr();
            if (surface) {
                surface->generatePoints(componentsFile);
            }
        }

        componentsFile.close();
        std::cout << "Component data generation completed successfully." << std::endl;
}
