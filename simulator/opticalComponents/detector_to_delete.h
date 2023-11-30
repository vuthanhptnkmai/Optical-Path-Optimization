#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
#include <Eigen/Dense>

// check if they're necessary 
#include <vector>
#include <algorithm>
#include <iostream>

#include "../ray.h"
#include "../../utils/vec3.h"
#include "../opticalComponent.h"
#include "../opticalSurface.h"
#include "../opticalSurfaces/PlanarRectangle.h"

template<typename T, typename U>
class Detector : public OpticalComponent<T, U> {
private:
    int pixelWidth, pixelHeight;
    std::vector<std::vector<U>> pixelData; // 2D array to store intensity for each pixel

public:
    Detector(std::unique_ptr<PlanarSurface<T, U>> surface, int pixelWidth, int pixelHeight);

    // Placeholder function for handling light, to be implemented later.
    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override;
    void outputImage(const std::string& filename) {
        // Implementation to save pixel data to an image file
        // This can be complex depending on theimage format and the data processing required
    } 
};

// Definitions

template<typename T, typename U>
Detector<T, U>::Detector(std::unique_ptr<PlanarSurface<T, U>> surface, int pixelWidth = 512, int pixelHeight = 512)
    : OpticalComponent<T, U>(std::move(surface)), pixelWidth(pixelWidth), pixelHeight(pixelHeight), pixelData(pixelWidth, std::vector<U>(pixelHeight, 0)) {
        if(dynamic_cast<PlanarSurface<T, U>*>(this->surface.get()) == nullptr) {
            throw std::runtime_error("Detector expects a PlanarSurface type. Provided surface type is invalid.");
        }
    }

template<typename T, typename U>
void Detector<T, U>::handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) {
    // Assuming a planar detector for simplicity
    vec3<T> localCoords = intersectionPoint - this->getPosition();
    vec3<T> normal = this->getNormal().normalized();
    
    // Project the intersection point onto the detector's plane
    T x = localCoords.dot(normal.cross(vec3<T>(0,1,0)));
    T y = localCoords.dot(normal.cross(vec3<T>(1,0,0)));

    // Normalize these coordinates to the range [0,1]
    x = (x / this->surface->getWidth()) + 0.5;
    y = (y / this->surface->getHeight()) + 0.5;

    // Convert normalized coordinates to pixel indices
    int pixelX = std::min(std::max(int(x * pixelWidth), 0), pixelWidth - 1);
    int pixelY = std::min(std::max(int(y * pixelHeight), 0), pixelHeight - 1);

    // Increment the intensity at the pixel
    // Assuming the intensity is 1 for each ray for simplicity
    pixelData[pixelX][pixelY] += 1;
}

void Detector<T, U>::adjustSurfaceToFitPixelGrid(int pixelWidth, int pixelHeight) {
    // Calculate aspect ratios
    T surfaceAspectRatio = this->surface->getWidth() / this->surface->getHeight();
    T pixelGridAspectRatio = static_cast<T>(pixelWidth) / static_cast<T>(pixelHeight);

    // Adjust surface dimensions
    T newWidth = this->surface->getWidth();
    T newHeight = this->surface->getHeight();
    if (pixelGridAspectRatio > surfaceAspectRatio) {
        // Adjust width
        newWidth = newHeight * pixelGridAspectRatio;
    } else {
        // Adjust height
        newHeight = newWidth / pixelGridAspectRatio;
    }

    // Create a new surface with adjusted dimensions
    auto newPosition = this->surface->getPosition();
    auto newNormal = this->surface->getNormal();
    auto newSurface = std::make_unique<PlanarRectangle<T, U>>(newPosition, newNormal, newWidth, newHeight);
    this->surface = std::move(newSurface);

    // Update pixel grid dimensions
    this->pixelWidth = pixelWidth;
    this->pixelHeight = pixelHeight;
    this->pixelData = Eigen::MatrixXf::Zero(pixelHeight, pixelWidth);
}

// Adjust the rest of the Detector class methods accordingly
