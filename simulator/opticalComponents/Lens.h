#pragma once

#include <iostream>
#include <stdexcept>
#include <memory> 

#include "../../utils/vec3.h"
#include "../opticalComponent.h"
#include "../opticalSurface.h"
#include "../ray.h"

#include <iostream>

template<typename T, typename U>
class Lens : public OpticalComponent<T, U> {
public:
    Lens(std::unique_ptr<OpticalSurface<T, U>> surface)
		: OpticalComponent<T, U>(std::move(surface), OpticalComponentType::Lens) {}
    virtual ~Lens() = default;

    virtual void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) = 0;
};

template<typename T, typename U>
class ThinLens : public Lens<T, U> {
// private:
public:
    U focalLength; // focal length of the lens for thin lens approximation
    void refract_approx(Ray<T, U>& ray, const vec3<T>& intersectionPoint);

public:
    ThinLens(std::unique_ptr<PlanarSurface<T, U>> surface, U focalLength);

    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override; 
};

// Definitions

template<typename T, typename U>
ThinLens<T, U>::ThinLens(std::unique_ptr<PlanarSurface<T, U>> surface, U focalLength)
    : Lens<T, U>(std::move(surface)), focalLength(focalLength) {
        if(dynamic_cast<PlanarSurface<T, U>*>(this->surface.get()) == nullptr) {
            throw std::runtime_error("ThinLens expects a PlanarSurface type. Provided surface type is invalid.");
        }
    }

template<typename T, typename U>
void ThinLens<T, U>::refract_approx(Ray<T, U>& ray, const vec3<T>& intersectionPoint) {
    vec3<T> O = ray.position;
    vec3<T> D = ray.direction;
    vec3<T> P = this->surface->getPosition();
    vec3<T> N = -this->surface->getNormal();

    vec3<T> PR = P - O; // principal ray from the light source to the center of the lens        
    T objectDistance = abs(PR.dot(N)); // distance from the object to the lens
    T imageDistance = 1.0 / ((1.0 / focalLength) - (1.0 / objectDistance)); // distance to the image using the thin lens approximation formula
    
    vec3<T> I = P + imageDistance * PR.normalized(); // calculate the image position

    ray.position = intersectionPoint; // change the position of the ray to the intersection of the ray with the lens

    if (imageDistance < 0) { // Virtual image scenario
        ray.direction = (intersectionPoint - I).normalized(); // reverse the direction towards the virtual image
    } else {
        ray.direction = (I - intersectionPoint).normalized(); // change the direction of the ray to the normalized vector from that intersection point to the image
    }
}

template<typename T, typename U>
void ThinLens<T, U>::handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) {
    refract_approx(ray, intersectionPoint);
}

// Extensions 

template<typename T, typename U>
class ThickLens : public Lens<T, U> {
public:
    ThickLens(std::unique_ptr<OpticalSurface<T, U>> surface)
	    : Lens<T, U>(std::move(surface)) {}

    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override;
};

template<typename T, typename U>
void ThickLens<T, U>::handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) {
    throw std::runtime_error("ThickLens::handleLight has not been implemented yet. TODO: Add implementation.");
}