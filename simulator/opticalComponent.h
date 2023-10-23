#pragma once

#include <memory> 

#include "../utils/vec3.h"
#include "opticalSurface.h"
#include "ray.h"

template<typename T, typename U>
class OpticalComponent {
protected:
    std::unique_ptr<OpticalSurface<T, U>> surface; 

public:
    OpticalComponent(std::unique_ptr<OpticalSurface<T, U>> surface)
        : surface(std::move(surface)) {}
    virtual ~OpticalComponent() = default;

    vec3<T> getPosition() const { return surface->getPosition(); }
    vec3<T> getNormal() const { return surface->getNormal(); }
    OpticalSurface<T, U>* getSurface() const { return surface.get(); }


    virtual void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) = 0;
};