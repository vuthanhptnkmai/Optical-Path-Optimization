#pragma once

#include "../utils/vec3.h"

// Forward declare Photon for the handleLight function
template<typename T, typename U>
class Photon;

template<typename T>
class OpticalComponent {
protected:
    vec3<T> position;
    vec3<T> normal;       
    vec3<T> dimensions;

public:
    OpticalComponent(const vec3<T>& position, const vec3<T>& normal, const vec3<T>& dimensions);

    // Getter functions
    vec3<T> getPosition() const { return position; }
    vec3<T> getNormal() const { return normal; }
    vec3<T> getDimensions() const { return dimensions; }

    // Setter functions
    void setPosition(const vec3<T>& newPosition) { position = newPosition; }
    void setNormal(const vec3<T>& newNormal) { normal = newNormal; }
    void setDimensions(const vec3<T>& newDimensions) { dimensions = newDimensions; }

    // Virtual function to handle light interaction
    virtual void handleLight(Photon<T, U> *photon) = 0;
};

template<typename T>
OpticalComponent<T>::OpticalComponent(const vec3<T>& position, const vec3<T>& normal, const vec3<T>& dimensions)
    : position(position), normal(normal), dimensions(dimensions) {}

template<typename T>
class RectangularComponent : public OpticalComponent<T> {
public:
    RectangularComponent(const vec3<T>& position, const vec3<T>& normal, const vec3<T>& dimensions)
        : OpticalComponent<T>(position, normal, dimensions) {}

    void handleLight(Photon<T, U> *photon) override {
        // Handle light interaction specific to rectangular components
    }
};

// You can then add more components similar to RectangularComponent as needed.
