#pragma once

#include <memory> 

#include "../../utils/vec3.h"
#include "../opticalComponent.h"
#include "../opticalSurface.h"
#include "../ray.h"

template<typename T, typename U>
class Filter : public OpticalComponent<T, U> {
private:
    std::string filterType;
    U wavelengthRange;

public:
    // Filter() = default; // Default constructor

    // Placeholder function for handling light, to be implemented later.
    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override = 0;
};

template<typename T, typename U>
class BandPassFilter : public Filter<T, U> {
public:

    // Placeholder function for handling light, to be implemented later.
    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override {
        ray.position = ray.position;
        ray.direction = ray.direction 
        // TODO: Implement this function later.
    }
};

template<typename T, typename U>
class LowPassFilter : public Filter<T, U> {

    // Placeholder function for handling light, to be implemented later.
    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override {
        ray.position = ray.position;
        ray.direction = ray.direction 
        // TODO: Implement this function later.
    }
};

template<typename T, typename U>
class HighPassFilter : public Filter<T, U> {

    // Placeholder function for handling light, to be implemented later.
    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override {
        ray.position = ray.position;
        ray.direction = ray.direction 
        // TODO: Implement this function later.
    }
};

