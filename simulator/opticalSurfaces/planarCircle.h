#pragma once

#include <utility>
#include <vector>
#include <math.h>

#include "../ray.h"
#include "../opticalSurface.h"
#include "../../utils/vec3.h"

template<typename T, typename U>
class PlanarCircle : public PlanarSurface<T, U> {
private:
    T radius;

public:
    PlanarCircle(const vec3<T>& position, const vec3<T>& normal, T diameter)
        : PlanarSurface<T, U>(position, normal), radius(diameter / 2.0) {}
    
    T getRadius() const { return radius; }

    std::pair<bool, vec3<T>> intersects(const Ray<T, U>& ray) const override;
    // void generatePoints(const std::string& filename) const;
    void generatePoints(std::ofstream& outFile) const;
};

// Definitions

template<typename T, typename U>
std::pair<bool, vec3<T>> PlanarCircle<T, U>::intersects(const Ray<T, U>& ray) const {
    vec3<T> intersectionPoint = this->getIntersectionPoint(ray);

    T distanceToCenter = (intersectionPoint - this->position).norm();
    bool doesIntersect = distanceToCenter <= radius;

    return {doesIntersect, intersectionPoint};
}

template<typename T, typename U>
void PlanarCircle<T, U>::generatePoints(std::ofstream& outFile) const {
    if (!outFile) {
        throw std::runtime_error("Output stream is not open.");
    }

    vec3<T> center = this->position;
    vec3<T> normal = this->normal;

    int numPoints = static_cast<int>(radius * 2 * M_PI * 9);

    vec3<T> defaultNormal(0, 1, 0);
    vec3<T> rotationAxis = defaultNormal.cross(normal).normalized();

    T cosTheta = defaultNormal.dot(normal);
    T theta = std::acos(cosTheta);

    for (int i = 0; i <= numPoints; ++i) {
        T t = i * 2 * M_PI / numPoints;
        vec3<T> point(radius * std::cos(t), 0, radius * std::sin(t));

        vec3<T> rotatedPoint = point * cosTheta
                            + rotationAxis.cross(point) * std::sin(theta)
                            + rotationAxis * rotationAxis.dot(point) * (1 - cosTheta);
        vec3<T> finalPoint = rotatedPoint + center;
        outFile << finalPoint(0) << " " << finalPoint(1) << " " << finalPoint(2) << std::endl;
    }

    outFile << "\n\n" << std::endl;
}
