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