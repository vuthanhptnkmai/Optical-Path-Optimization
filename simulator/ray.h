#pragma once

#include "../utils/vec3.h"

// Declaration
template<typename T, typename U>
class Ray {
private:
    vec3<T> position;
    vec3<T> direction;
    U wavelength;
    U intensity;

public:
    Ray(const vec3<T>& position, const vec3<T>& direction, U wavelength, U intensity);

    const vec3<T>& getPosition() const;
    const vec3<T>& getDirection() const;
    U getWavelength() const;
    U getIntensity() const;

    void setPosition(const vec3<T>& pos);
    void setDirection(const vec3<T>& dir);
    void setWavelength(U wl);
    void setIntensity(U inten);
};

// Definitions

template<typename T, typename U>
Ray<T, U>::Ray(const vec3<T>& position, const vec3<T>& direction, U wavelength, U intensity)
    : position(position), direction(direction.normalized()), wavelength(wavelength), intensity(intensity) {}

template<typename T, typename U>
const vec3<T>& Ray<T, U>::getPosition() const {
    return position;
}

template<typename T, typename U>
const vec3<T>& Ray<T, U>::getDirection() const {
    return direction;
}

template<typename T, typename U>
U Ray<T, U>::getWavelength() const {
    return wavelength;
}

template<typename T, typename U>
U Ray<T, U>::getIntensity() const {
    return intensity;
}

template<typename T, typename U>
void Ray<T, U>::setPosition(const vec3<T>& pos) {
    position = pos;
}

template<typename T, typename U>
void Ray<T, U>::setDirection(const vec3<T>& dir) {
    direction = dir;
}

template<typename T, typename U>
void Ray<T, U>::setWavelength(U wl) {
    wavelength = wl;
}

template<typename T, typename U>
void Ray<T, U>::setIntensity(U inten) {
    intensity = inten;
}
