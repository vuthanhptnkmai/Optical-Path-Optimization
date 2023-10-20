#pragma once

#include <memory> 

#include "../../utils/vec3.h"
#include "../opticalComponent.h"
#include "../opticalSurface.h"
#include "../ray.h"

template<typename T, typename U>
class Mirror : public OpticalComponent<T, U> {
private:
    Ray<T, U> reflect(const Ray<T, U>& incidentRay, const vec3<T>& normal);

public:
    Mirror(std::unique_ptr<OpticalSurface<T, U>> surface)
        : OpticalComponent<T, U>(std::move(surface)) {}

    virtual void handleLight(Ray<T, U>& ray) = 0;
};

// Definitions

template<typename T, typename U>
Ray<T, U> Mirror<T, U>::reflect(const Ray<T, U>& incidentRay, const vec3<T>& normal) {
    vec3<T> reflectedDirection = incidentRay.getDirection() - 2 * incidentRay.getDirection().dot(normal) * normal;
    reflectedDirection.normalize();  // ensure the direction is a unit vector
    return Ray<T, U>(incidentRay.getPosition(), reflectedDirection, incidentRay.getWavelength(), incidentRay.getIntensity());
}

template<typename T, typename U>
class PlanarMirror : public Mirror<T, U> {
public:
    PlanarMirror(std::unique_ptr<PlanarSurface<T, U>> surface)
        : Mirror<T, U>(std::move(surface)) {}
    
    void handleLight(Ray<T, U>& ray) override;
};

template<typename T, typename U>
void PlanarMirror<T, U>::handleLight(Ray<T, U>& ray) {
    ray = reflect(ray, this->getNormal());
}

// Extensions

template<typename T, typename U>
class ConvexMirror : public Mirror<T, U> {
public:
    ConvexMirror(std::unique_ptr<SphericalSurface<T, U>> surface)
        : Mirror<T, U>(std::move(surface)) {}

    void handleLight(Ray<T, U>& ray) override;
};

template<typename T, typename U>
void ConvexMirror<T, U>::handleLight(Ray<T, U>& ray) {
    std::cout << "The handleLight method needs to be implemented for ConvexMirror." << std::endl;
}

template<typename T, typename U>
class ConcaveMirror : public Mirror<T, U> {
public:
    ConcaveMirror(std::unique_ptr<SphericalSurface<T, U>> surface)
        : Mirror<T, U>(std::move(surface)) {}

    void handleLight(Ray<T, U>& ray) override;
};

template<typename T, typename U>
void ConcaveMirror<T, U>::handleLight(Ray<T, U>& ray) {
    std::cout << "The handleLight method needs to be implemented for ConcaveMirror." << std::endl;
}