#pragma once

#include "../ray.h"
#include "../../utils/vec3.h"
#include "../opticalSurface.h"


template<typename T, typename U>
class Rectangle : public PlaneSurface<T, U> {
protected:
    T width, height;
    vec3<T> widthDirection, heightDirection;  // widthDirection is provided, heightDirection is computed

public:
    Rectangle(const vec3<T>& position, const vec3<T>& normal, T width, T height, const vec3<T>& widthDirection);

    bool intersects(const Ray<T, U>& ray) const override;
};

// Definitions

template<typename T, typename U>
Rectangle<T, U>::Rectangle(const vec3<T>& position, const vec3<T>& normal, T width, T height, const vec3<T>& widthDirection)
    : width(width), height(height), widthDirection(widthDirection.normalized()), heightDirection(normal.cross(widthDirection).normalized()) {
    this->position = position;
    this->normal = normal.normalized();
}

template<typename T, typename U>
bool Rectangle<T, U>::intersects(const Ray<T, U>& ray) const {
    vec3<T> intersectionPoint = this->getIntersectionPoint(ray);
    vec3<T> D = intersectionPoint - this->position;

    T u = D.dot(widthDirection);
    T v = D.dot(heightDirection);

    return (std::abs(u) <= width / 2.0) && (std::abs(v) <= height / 2.0);
}
