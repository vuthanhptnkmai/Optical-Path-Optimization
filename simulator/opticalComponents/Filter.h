#pragma once

#include <memory> 

#include "../../utils/vec3.h"
#include "../opticalComponent.h"
#include "../opticalSurface.h"
#include "../ray.h"

template<typename T, typename U>
class Filter : public OpticalComponent<T, U> {
private:
    std::string filterType;
    U wavelengthRange;

public:
    Filter(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape, const std::string& filterType, U wavelengthRange);

    void handleLight(Ray<T, U>& ray) override;
};

// Definitions

template<typename T, typename U>
Filter<T, U>::Filter(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape, const std::string& filterType, U wavelengthRange)
    : OpticalComponent<T, U>(position, normal, std::move(shape)), filterType(filterType), wavelengthRange(wavelengthRange) {}

template<typename T, typename U>
void Filter<T, U>::handleLight(Ray<T, U>& ray) {
    // Specific behavior for Filter
}