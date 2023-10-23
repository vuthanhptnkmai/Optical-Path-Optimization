#pragma once

#include <utility>

#include "../ray.h"
#include "../opticalSurface.h"
#include "../../utils/vec3.h"

template<typename T, typename U>
class PlanarRectangle : public PlanarSurface<T, U> {
protected:
    T width, height;
    vec3<T> widthDirection, heightDirection;  // widthDirection is provided, heightDirection is computed

public:
    PlanarRectangle(const vec3<T>& position, const vec3<T>& normal, T width, T height, const vec3<T>& widthDirection);

    std::pair<bool, vec3<T>> intersects(const Ray<T, U>& ray) const override;
    void generatePoints() const override;
};

// Definitions

template<typename T, typename U>
PlanarRectangle<T, U>::PlanarRectangle(const vec3<T>& position, const vec3<T>& normal, T width, T height, const vec3<T>& widthDirection)
    : PlanarSurface<T, U>(position, normal), width(width), height(height), 
      widthDirection(widthDirection.normalized()), 
      heightDirection(normal.cross(widthDirection).normalized()) {}
        
template<typename T, typename U>
std::pair<bool, vec3<T>> PlanarRectangle<T, U>::intersects(const Ray<T, U>& ray) const {
    vec3<T> intersectionPoint = this->getIntersectionPoint(ray);
    vec3<T> D = intersectionPoint - this->position;

    T u = D.dot(widthDirection);
    T v = D.dot(heightDirection);

    bool doesIntersect = (std::abs(u) <= width / 2.0) && (std::abs(v) <= height / 2.0);

    return {doesIntersect, intersectionPoint};
}

template<typename T, typename U>
void PlanarRectangle<T, U>::generatePoints() const {
    throw std::runtime_error("PlanarRectangle::generatePoints has not been implemented yet. TODO: Add implementation.");
}