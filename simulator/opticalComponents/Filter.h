#pragma once

#include <memory> 

#include "../../utils/vec3.h"
#include "../opticalComponent.h"
#include "../opticalSurface.h"
#include "../ray.h"

template<typename T, typename U>
class Filter : public OpticalComponent<T, U> {
private:
    U minWavelength, maxWavelength; // Range for the filter

public:
    Filter(std::unique_ptr<PlanarSurface<T, U>> surface, U minWavelength, U maxWavelength);

    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override; 
};

// Definitions

template<typename T, typename U>
Filter<T, U>::Filter(std::unique_ptr<PlanarSurface<T, U>> surface, U minWavelength, U maxWavelength)
    : OpticalComponent<T, U>(std::move(surface), OpticalComponentType::Filter), minWavelength(minWavelength), maxWavelength(maxWavelength) {
        if(dynamic_cast<PlanarSurface<T, U>*>(this->surface.get()) == nullptr) {
            throw std::runtime_error("Filter expects a PlanarSurface type. Provided surface type is invalid.");
        }
    }

template<typename T, typename U>
void Filter<T, U>::handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) {
    if (ray.getWavelength() < minWavelength || ray.getWavelength() > maxWavelength) {
        ray.setIntensity(0); // only pass rays within the specified wavelength range
    } else { 
        ray.setPosition(intersectionPoint);
    }
}
