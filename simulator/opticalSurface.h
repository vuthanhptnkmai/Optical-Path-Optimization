#pragma once

#include <stdexcept> // for std::runtime_error

#include "ray.h"
#include "../utils/vec3.h"

template<typename T, typename U>
class OpticalSurface {
public:
    virtual ~OpticalSurface() = default; // the compiler will generate the default implementation for the destructor

    virtual vec3<T> getIntersectionPoint(const Ray<T, U>& ray) const = 0; 
    virtual bool intersects(const Ray<T, U>& ray) const = 0;
};

template<typename T, typename U>
class PlanarSurface : public OpticalSurface<T, U> {
protected:
    vec3<T> position;   // position or center of the optical surface
    vec3<T> normal;     // normal or principal axis of the optical surface

public:
    vec3<T> getPosition() const { return position; }
    vec3<T> getNormal() const { return normal; }

    vec3<T> getIntersectionPoint(const Ray<T, U>& ray) const; 
    virtual bool intersects(const Ray<T, U>& ray) const = 0;
};

// Definitions

// Note: write code to handle the case where the ray is parallel to the optical component
template<typename T, typename U>
vec3<T> PlanarSurface<T, U>::getIntersectionPoint(const Ray<T, U>& ray) const {
    T intersectionDistance = (position - ray.getPosition()).dot(normal) / ray.getDirection().dot(normal);
    return ray.getPosition() + intersectionDistance * ray.getDirection();
}

