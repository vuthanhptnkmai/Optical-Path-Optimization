#pragma once

#include "../utils/vec3.h"

// Declaration and Definitions
template<typename T, typename U>
class Ray {
private:
    U wavelength;
    U intensity;

public:
    // these attributes are public for direct and efficient access
    // maintain the direction as a normalized vector when modifying it
    vec3<T> position;
    vec3<T> direction;

    Ray(const vec3<T>& position, const vec3<T>& direction, U wavelength, U intensity)
        : position(position), direction(direction.normalized()), wavelength(wavelength), intensity(intensity) {}

    const vec3<T>& getPosition() const { return position; }
    const vec3<T>& getDirection() const { return direction; }
    U getWavelength() const { return wavelength; }
    U getIntensity() const { return intensity; }

    void setPosition(const vec3<T>& newPosition) { position = newPosition; }
    void setDirection(const vec3<T>& newDirection) { direction = newDirection.normalized(); }
    void setWavelength(U newWavelength) { wavelength = newWavelength; }
    void setIntensity(U newIntensity) { intensity = newIntensity; }
};
