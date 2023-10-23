#pragma once

#include <stdexcept>
#include <memory> 

#include "../../utils/vec3.h"
#include "../opticalComponent.h"
#include "../opticalSurface.h"
#include "../ray.h"

template<typename T, typename U>
class Mirror : public OpticalComponent<T, U> {
protected:
    void reflect(Ray<T, U>& incidentRay, const vec3<T>& normal, const vec3<T>& intersectionPoint);

public:
    Mirror(std::unique_ptr<OpticalSurface<T, U>> surface)
        : OpticalComponent<T, U>(std::move(surface)) {}
    virtual ~Mirror() = default;

    virtual void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override = 0;
};

template<typename T, typename U>
class PlanarMirror : public Mirror<T, U> {
public:
    PlanarMirror(std::unique_ptr<OpticalSurface<T, U>> surface);

    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override;
};

// Definitions

template<typename T, typename U>
void Mirror<T, U>::reflect(Ray<T, U>& incidentRay, const vec3<T>& normal, const vec3<T>& intersectionPoint) {
    vec3<T> reflectedDirection = incidentRay.getDirection() - 2 * incidentRay.getDirection().dot(normal) * normal;
    
    incidentRay.position = intersectionPoint;
    // reflectedDirection.normalize();  // ensure the direction is a unit vector
    // incidentRay.direction = reflectedDirection;
    incidentRay.direction = reflectedDirection.normalized();
}

template<typename T, typename U>
PlanarMirror<T, U>::PlanarMirror(std::unique_ptr<OpticalSurface<T, U>> surface)
    : Mirror<T, U>(std::move(surface)) {
        if(dynamic_cast<PlanarSurface<T, U>*>(this->surface.get()) == nullptr) {
            throw std::runtime_error("PlanarMirror expected a PlanarSurface. Provided surface type is invalid.");
        }
    }

template<typename T, typename U>
void PlanarMirror<T, U>::handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) {
    this->reflect(ray, this->getNormal(), intersectionPoint);
}

// Extensions

template<typename T, typename U>
class ConvexMirror : public Mirror<T, U> {
public:
    ConvexMirror(std::unique_ptr<OpticalSurface<T, U>> surface);

        void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override;

};

template<typename T, typename U>
class ConcaveMirror : public Mirror<T, U> {
public:
    ConcaveMirror(std::unique_ptr<OpticalSurface<T, U>> surface);

    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override;
};

template<typename T, typename U>
ConvexMirror<T, U>::ConvexMirror(std::unique_ptr<OpticalSurface<T, U>> surface)
    : Mirror<T, U>(std::move(surface)) {
        if(dynamic_cast<SphericalSurface<T, U>*>(this->surface.get()) == nullptr) {
            throw std::runtime_error("ConvexMirror expects a SphericalSurface type. Provided surface type is invalid.");
        }
    }

template<typename T, typename U>
void ConvexMirror<T, U>::handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) {
    throw std::runtime_error("ConvexMirror::handleLight has not been implemented yet. TODO: Add implementation.");
}

template<typename T, typename U>
ConcaveMirror<T, U>::ConcaveMirror(std::unique_ptr<OpticalSurface<T, U>> surface)
    : Mirror<T, U>(std::move(surface)) {
        if(dynamic_cast<SphericalSurface<T, U>*>(this->surface.get()) == nullptr) {
            throw std::runtime_error("ConcaveMirror expects a SphericalSurface type. Provided surface type is invalid.");
        }
    }

template<typename T, typename U>
void ConcaveMirror<T, U>::handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) {
    throw std::runtime_error("ConcaveMirror::handleLight has not been implemented yet. TODO: Add implementation.");
}