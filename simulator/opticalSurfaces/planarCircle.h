#pragma once

#include "../ray.h"
#include "../../utils/vec3.h"
#include "../opticalSurface.h"

template<typename T, typename U>
class Circle : public PlaneSurface<T, U> {
protected:
    T radius;

public:
    Circle(const vec3<T>& position, const vec3<T>& normal, T diameter);
    T getRadius() const;

    bool intersects(const Ray<T, U>& ray) const override;
};

// Definitions

template<typename T, typename U>
Circle<T, U>::Circle(const vec3<T>& position, const vec3<T>& normal, T diameter)
    : radius(diameter / 2.0) {
    this->position = position;
    this->normal = normal.normalized();
}

template<typename T, typename U>
T Circle<T, U>::getRadius() const {
    return radius;
}

template<typename T, typename U>
bool Circle<T, U>::intersects(const Ray<T, U>& ray) const {
    vec3<T> intersectionPoint = this->getIntersectionPoint(ray);
    T distanceToCenter = (intersectionPoint - this->position).norm();
    return distanceToCenter <= radius;
}