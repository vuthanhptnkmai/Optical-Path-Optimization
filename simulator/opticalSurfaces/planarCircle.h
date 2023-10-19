#pragma once

#include "../ray.h"
#include "../../utils/vec3.h"
#include "../opticalSurface.h"

template<typename T, typename U>
class PlanarCircle : public PlanarSurface<T, U> {
private:
    T radius;

public:
    PlanarCircle(const vec3<T>& position, const vec3<T>& normal, T diameter);
    T getRadius() const { return radius; }

    bool intersects(const Ray<T, U>& ray) const override;
};

// Definitions

template<typename T, typename U>
PlanarCircle<T, U>::PlanarCircle(const vec3<T>& position, const vec3<T>& normal, T diameter)
    : radius(diameter / 2.0) {
    this->position = position;
    this->normal = normal.normalized();
}

template<typename T, typename U>
bool PlanarCircle<T, U>::intersects(const Ray<T, U>& ray) const {
    vec3<T> intersectionPoint = this->getIntersectionPoint(ray);
    T distanceToCenter = (intersectionPoint - this->position).norm();
    return distanceToCenter <= radius;
}
