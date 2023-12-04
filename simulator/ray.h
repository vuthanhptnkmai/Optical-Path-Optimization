#pragma once

#include "../utils/vec3.h"

/**
 * Represents a ray in an optical system.
 * 
 * The Ray class encapsulates the properties of a ray of light, including its position, direction,
 * wavelength, and intensity. The direction vector is maintained as a normalized vector to ensure
 * accurate representation of the ray's path.
 * 
 * Template Parameters:
 *  - T: Type for position and direction vector components (e.g., float, double).
 *  - U: Type for wavelength and intensity (e.g., int, float).
 */
template<typename T, typename U>
class Ray {
private:
    U wavelength; // Wavelength of the ray in nanometers (nm)
    U intensity;  // Intensity of the ray, representing its strength or amplitude

public:
    vec3<T> position;  // Position of the ray in 3D space
    vec3<T> direction; // Direction of the ray as a normalized vector

    /**
     * Constructor for the Ray class.
     * 
     * @param position The starting position of the ray in 3D space.
     * @param direction The initial direction of the ray, which will be normalized.
     * @param wavelength The wavelength of the ray, typically in nanometers (nm).
     * @param intensity The intensity of the ray.
     */
    Ray(const vec3<T>& position, const vec3<T>& direction, U wavelength, U intensity)
        : position(position), direction(direction.normalized()), wavelength(wavelength), intensity(intensity) {}

    // Getters
    const vec3<T>& getPosition() const { return position; }
    const vec3<T>& getDirection() const { return direction; }
    U getWavelength() const { return wavelength; }
    U getIntensity() const { return intensity; }

    // Setters
    void setPosition(const vec3<T>& newPosition) { position = newPosition; }
    void setDirection(const vec3<T>& newDirection) { direction = newDirection.normalized(); }
    void setWavelength(U newWavelength) { wavelength = newWavelength; }
    void setIntensity(U newIntensity) { intensity = newIntensity; }
};
