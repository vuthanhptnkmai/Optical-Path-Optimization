#pragma once

#include <memory> 

#include "../utils/vec3.h"
#include "shape.h"
#include "ray.h"

template<typename T, typename U>
class OpticalComponent {
protected:
    vec3<T> position;
    vec3<T> normal;
    std::unique_ptr<Shape<T>> shape;

public:
    OpticalComponent(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape);

    vec3<T> getPosition() const;
    vec3<T> getNormal() const;
    Shape<T>* getShape() const;

    void setPosition(const vec3<T>& newPosition);
    void setNormal(const vec3<T>& newNormal);

    virtual void handleLight(Ray<T, U>& ray);
};

template<typename T, typename U>
class Mirror : public OpticalComponent<T, U> {
public:
    Mirror(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape);

    void handleLight(Ray<T, U>& ray) override;
};

template<typename T, typename U>
class Filter : public OpticalComponent<T, U> {
private:
    std::string filterType;
    U wavelengthRange;

public:
    Filter(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape, const std::string& filterType, U wavelengthRange);

    void handleLight(Ray<T, U>& ray) override;
};

template<typename T, typename U>
class Lens : public OpticalComponent<T, U> {
public:
    Lens(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape);

    void handleLight(Ray<T, U>& ray) override;
};

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
OpticalComponent<T, U>::OpticalComponent(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape)
    : position(position), normal(normal), shape(std::move(shape)) {}

template<typename T, typename U>
vec3<T> OpticalComponent<T, U>::getPosition() const { return position; }

template<typename T, typename U>
vec3<T> OpticalComponent<T, U>::getNormal() const { return normal; }

template<typename T, typename U>
Shape<T>* OpticalComponent<T, U>::getShape() const { return shape.get(); }

template<typename T, typename U>
void OpticalComponent<T, U>::setPosition(const vec3<T>& newPosition) { position = newPosition; }

template<typename T, typename U>
void OpticalComponent<T, U>::setNormal(const vec3<T>& newNormal) { normal = newNormal; }

template<typename T, typename U>
void OpticalComponent<T, U>::handleLight(Ray<T, U>& ray) {
    // Default behavior
}

template<typename T, typename U>
Mirror<T, U>::Mirror(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape)
    : OpticalComponent<T, U>(position, normal, std::move(shape)) {}

template<typename T, typename U>
void Mirror<T, U>::handleLight(Ray<T, U>& ray) {
    // Specific behavior for Mirror
}

template<typename T, typename U>
Filter<T, U>::Filter(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape, const std::string& filterType, U wavelengthRange)
    : OpticalComponent<T, U>(position, normal, std::move(shape)), filterType(filterType), wavelengthRange(wavelengthRange) {}

template<typename T, typename U>
void Filter<T, U>::handleLight(Ray<T, U>& ray) {
    // Specific behavior for Filter
}

template<typename T, typename U>
Lens<T, U>::Lens(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape)
    : OpticalComponent<T, U>(position, normal, std::move(shape)) {}

template<typename T, typename U>
void Lens<T, U>::handleLight(Ray<T, U>& ray) {
    // Specific behavior for Lens
}

template<typename T, typename U>
Detector<T, U>::Detector(const vec3<T>& position, const vec3<T>& normal, std::unique_ptr<Shape<T>> shape, int pixelWidth, int pixelHeight)
    : OpticalComponent<T, U>(position, normal, std::move(shape)), pixelWidth(pixelWidth), pixelHeight(pixelHeight) {}

template<typename T, typename U>
void Detector<T, U>::handleLight(Ray<T, U>& ray) {
    // Specific behavior for Detector
}