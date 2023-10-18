#pragma once

#include <stdexcept> // for std::runtime_error

#include "ray.h"
#include "../utils/vec3.h"


template<typename T, typename U>
class OpticalSurface {
public:
    virtual ~OpticalSurface() = default; // the compiler will generate the default implementation for the destructor

    virtual bool intersects(const Ray<T, U>& ray) const = 0;
};

template<typename T, typename U>
class PlaneSurface : public OpticalSurface<T, U> {
protected: // Note: could be changed to private -> see later for more information
    vec3<T> position;   // position or center of the optical surface
    vec3<T> normal;     // normal or principal axis of the optical surface

public:
    vec3<T> getPosition() const;
    vec3<T> getNormal() const;

    vec3<T> getIntersectionPoint(const Ray<T, U>& ray) const; 
    bool intersects(const Ray<T, U>& ray) const override;
};

// Definitions

template<typename T, typename U>
vec3<T> PlaneSurface<T, U>::getPosition() const {
    return position;
}

template<typename T, typename U>
vec3<T> PlaneSurface<T, U>::getNormal() const {
    return normal;
}

// Note: write code to handle the case where the ray is parallel to the optical component
template<typename T, typename U>
vec3<T> PlaneSurface<T, U>::getIntersectionPoint(const Ray<T, U>& ray) const {
    T intersectionDistance = (position - ray.getPosition()).dot(normal) / ray.getDirection().dot(normal);
    return ray.getPosition() + intersectionDistance * ray.getDirection();
}

template<typename T, typename U>
bool PlaneSurface<T, U>::intersects(const Ray<T, U>& ray) const {
    throw std::runtime_error("intersects function not implemented for PlaneSurface");
}
