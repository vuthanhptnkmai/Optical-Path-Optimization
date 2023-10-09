#pragma once

#include "../utils/vec3.h"

template<typename T, typename U>
class Ray {
public: // here it should be protected instead .. try out different methods for more information, write functions for getter and setter
    vec3<T> position;
    vec3<T> direction;
    U wavelength;
    U intensity;

public:
    Ray(const vec3<T>& position, const vec3<T>& direction, U wavelength, U intensity);
};

template<typename T, typename U>
Ray<T, U>::Ray(const vec3<T>& position, const vec3<T>& direction, U wavelength, U intensity)
    : position(position), direction(direction), wavelength(wavelength), intensity(intensity) {}
