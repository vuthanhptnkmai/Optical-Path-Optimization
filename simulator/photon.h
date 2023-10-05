#pragma once

#include "../utils/vec3.h"

template<typename T, typename U>
class Photon {
public: // here it should be protected instead .. try out different methods for more information
    vec3<T> position;
    vec3<T> direction;
    U wavelength;
    U intensity;

public:
    Photon(const vec3<T>& position, const vec3<T>& direction, U wavelength, U intensity);
};

template<typename T, typename U>
Photon<T, U>::Photon(const vec3<T>& position, const vec3<T>& direction, U wavelength, U intensity)
    : position(position), direction(direction), wavelength(wavelength), intensity(intensity) {}
