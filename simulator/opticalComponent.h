#pragma once

#include <memory> 

#include "../utils/vec3.h"
#include "opticalSurface.h"
#include "ray.h"

enum class OpticalComponentType {
    Mirror,
    Lens,
    Filter,
    Detector
};

template<typename T, typename U>
class OpticalComponent {
protected:
    std::unique_ptr<OpticalSurface<T, U>> surface; 
    OpticalComponentType type;  

public:
    OpticalComponent(std::unique_ptr<OpticalSurface<T, U>> surface, OpticalComponentType type)
        : surface(std::move(surface)), type(type) {}
    virtual ~OpticalComponent() = default;

    const vec3<T>& getPosition() const { return surface->getPosition(); }
    const vec3<T>& getNormal() const { return surface->getNormal(); }

    OpticalSurface<T, U>* getSurfacePtr() { return surface.get(); }
    OpticalComponentType getType() const { return type; } 

    virtual void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) = 0;
};