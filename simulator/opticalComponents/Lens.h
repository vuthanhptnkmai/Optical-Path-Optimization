#pragma once

#include <memory> 

#include "../../utils/vec3.h"
#include "../opticalComponent.h"
#include "../opticalSurface.h"
#include "../ray.h"

template<typename T, typename U>
class Lens : public OpticalComponent<T, U> {
protected:
    T focalLength; // focal length of the lens

public:
    Lens(std::unique_ptr<OpticalSurface<T, U>> surface, T focalLength)
			: OpticalComponent<T, U>(std::move(surface)), focalLength(focalLength) {}

    virtual void handleLight(Ray<T, U>& ray) = 0;
};

template<typename T, typename U>
class ThinLens : public Lens<T, U> {
public:
    ThinLens(std::unique_ptr<PlanarSurface<T, U>> surface, T focalLength)
        : Lens<T, U>(std::move(surface), focalLength) {}

    void handleLight(Ray<T, U>& ray) override = 0; // still pure virtual
};

template<typename T, typename U>
class ConvexThinLens : public ThinLens<T, U> {
public:
    ConvexThinLens(std::unique_ptr<OpticalSurface<T, U>> surface, T focalLength)
	    : ThinLens<T, U>(std::move(surface), std::abs(focalLength)) {}

    void handleLight(Ray<T, U>& ray) override;
};

template<typename T, typename U>
class ConcaveThinLens : public ThinLens<T, U> {
public:
    ConcaveThinLens(std::unique_ptr<OpticalSurface<T, U>> surface, T focalLength)
	    : ThinLens<T, U>(std::move(surface), -std::abs(focalLength)) {}

    void handleLight(Ray<T, U>& ray) override;
};

// Definitions

template<typename T, typename U>
void ConvexThinLens<T, U>::handleLight(Ray<T, U>& ray) {
    // Specific behavior for ConvexThinLens
}

template<typename T, typename U>
void ConcaveThinLens<T, U>::handleLight(Ray<T, U>& ray) {
    // Specific behavior for ConcaveThinLens
}

// Extensions 

template<typename T, typename U>
class ThickLens : public Lens<T, U> {
public:
    ThickLens(std::unique_ptr<OpticalSurface<T, U>> surface, T focalLength)
	    : Lens<T, U>(std::move(surface), focalLength) {}
    void handleLight(Ray<T, U>& ray) override;
};

template<typename T, typename U>
void ThickLens<T, U>::handleLight(Ray<T, U>& ray) {
    std::cout << "No specific implementation for ThickLens yet." << std::endl;
}