#pragma once

#include <stdexcept>
#include <utility>
#include <memory>

#include "ray.h"
#include "../utils/vec3.h"

template<typename T, typename U>
class OpticalSurface {
protected:
    vec3<T> position;   // position or center of the optical surface
    vec3<T> normal;     // normal or principal axis of the optical surface

public:
    OpticalSurface(const vec3<T>& position, const vec3<T>& normal) : position(position), normal(normal.normalized()) {}
    virtual ~OpticalSurface() = default;

    const vec3<T>& getPosition() const { return position; }
    const vec3<T>& getNormal() const { return normal; }

    virtual vec3<T> getIntersectionPoint(const Ray<T, U>& ray) const = 0; 
    virtual std::pair<bool, vec3<T>> intersects(const Ray<T, U>& ray) const = 0;
    virtual void generatePoints(const std::string& filename) const = 0;
};

template<typename T, typename U>
class PlanarSurface : public OpticalSurface<T, U> {
public:
    PlanarSurface(const vec3<T>& position, const vec3<T>& normal) : OpticalSurface<T, U>(position, normal) {}
    virtual ~PlanarSurface() = default;

    vec3<T> getIntersectionPoint(const Ray<T, U>& ray) const override;
    virtual std::pair<bool, vec3<T>> intersects(const Ray<T, U>& ray) const = 0;
    virtual void generatePoints(const std::string& filename) const = 0;
};

template<typename T, typename U>
vec3<T> PlanarSurface<T, U>::getIntersectionPoint(const Ray<T, U>& ray) const {
    T intersectionDistance = (this->position - ray.position).dot(this->normal) / ray.direction.dot(this->normal);
    return ray.position + intersectionDistance * ray.direction;
}

// Extensions 

template<typename T, typename U>
class SphericalSurface : public OpticalSurface<T, U> {
protected:
    T radius;  // radius of the spherical surface. positive for convex and negative for concave.

public:
    SphericalSurface(const vec3<T>& position, const vec3<T>& normal, T radius)
        : OpticalSurface<T, U>(position, normal), radius(radius) {}
    virtual ~SphericalSurface() = default;

    vec3<T> getIntersectionPoint(const Ray<T, U>& ray) const override;
    std::pair<bool, vec3<T>> intersects(const Ray<T, U>& ray) const override;
    virtual void generatePoints() const override;
};

template<typename T, typename U>
vec3<T> SphericalSurface<T, U>::getIntersectionPoint(const Ray<T, U>& ray) const {
    throw std::runtime_error("SphericalSurface::getIntersectionPoint has not been implemented yet. TODO: Add implementation.");
    return vec3<T>();  // just return a default vec3, to be changed later
}

template<typename T, typename U>
std::pair<bool, vec3<T>> SphericalSurface<T, U>::intersects(const Ray<T, U>& ray) const {
    throw std::runtime_error("SphericalSurface::intersects has not been implemented yet. TODO: Add implementation.");
    return {false, vec3<T>()};  // just return false for now, to be changed later
}

template<typename T, typename U>
void SphericalSurface<T, U>::generatePoints() const {
    throw std::runtime_error("SphericalSurface::generatePoints has not been implemented yet. TODO: Add implementation.");
}