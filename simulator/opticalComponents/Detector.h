#pragma once

#include <memory> 

#include "../../utils/vec3.h"
#include "../opticalComponent.h"
#include "../opticalSurface.h"
#include "../ray.h"

template<typename T, typename U>
class Detector : public OpticalComponent<T, U> {
private:
    int pixelWidth, pixelHeight;

public:
    Detector(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape, int pixelWidth, int pixelHeight);

    void handleLight(Ray<T, U>& ray) override;
};

// Definitions

template<typename T, typename U>
Detector<T, U>::Detector(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape, int pixelWidth, int pixelHeight)
    : OpticalComponent<T, U>(position, normal, std::move(shape)), pixelWidth(pixelWidth), pixelHeight(pixelHeight) {}

template<typename T, typename U>
void Detector<T, U>::handleLight(Ray<T, U>& ray) {
    // Specific behavior for Detector
}