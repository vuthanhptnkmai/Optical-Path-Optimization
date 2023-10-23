#pragma once

#include <memory> 

#include "../../utils/vec3.h"
#include "../opticalComponent.h"
#include "../opticalSurface.h"
#include "../ray.h"

template<typename T, typename U>
class Detector : public OpticalComponent<T, U> {
private:
    int pixelWidth, pixelHeight;

public:
public:
    // Detector() = default; // Default constructor

    // Placeholder function for handling light, to be implemented later.
    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override {
        ray.position = ray.position;
        ray.direction = ray.direction 
        // TODO: Implement this function later.
    }
};