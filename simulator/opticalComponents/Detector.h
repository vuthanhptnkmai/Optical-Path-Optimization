#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
#include "../../thirdParty/eigen-3.4.0/Eigen/Dense"

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
    Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic> pixelGrid;

    std::pair<int, int> mapToPixelGrid(const vec3<T>& intersectionPoint) const;
    void adjustSurfaceToFitPixelGrid();

public:
    Detector(std::unique_ptr<PlanarSurface<T, U>> surface, int pixelWidth = 512, int pixelHeight = 512)
        : OpticalComponent<T, U>(std::move(surface), OpticalComponentType::Detector), pixelWidth(pixelWidth), pixelHeight(pixelHeight),
          pixelGrid(Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>::Zero(pixelHeight, pixelWidth)) {
        if (dynamic_cast<PlanarSurface<T, U>*>(this->surface.get()) == nullptr) {
            throw std::runtime_error("Detector expects a PlanarSurface type. Provided surface type is invalid.");
        }
        adjustSurfaceToFitPixelGrid();
    }

    const Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>& getPixelGrid() const { return pixelGrid; }

    void handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) override;
    // void resetPixelGrid() { pixelGrid = Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>::Zero(pixelHeight, pixelWidth); }
    void resetPixelGrid() { pixelGrid.setZero(pixelHeight, pixelWidth); }
};

// Definitions

template<typename T, typename U>
void Detector<T, U>::adjustSurfaceToFitPixelGrid() {
    
    auto planarRect = dynamic_cast<PlanarRectangle<T, U>*>(this->surface.get());
    if (!planarRect) throw std::runtime_error("Surface is not a PlanarRectangle.");

    T width = planarRect->getWidth();
    T height = planarRect->getHeight();

    T pixelWidth = static_cast<T>(this->pixelWidth);
    T pixelHeight = static_cast<T>(this->pixelHeight);

    // Calculate aspect ratios
    T surfaceAspectRatio = width / height;
    T pixelGridAspectRatio = pixelWidth / pixelHeight;

    // Adjust surface dimensions
    if (pixelGridAspectRatio > surfaceAspectRatio) {
        height = width / pixelGridAspectRatio; // Adjust height
    } else if (pixelGridAspectRatio < surfaceAspectRatio) {
        width = height * pixelGridAspectRatio; // Adjust width

    }

    // Create a new surface with adjusted dimensions
    auto newSurface = std::make_unique<PlanarRectangle<T, U>>(planarRect->getPosition(), planarRect->getNormal(), width, height, planarRect->getWidthDirection());
    this->surface = std::move(newSurface);
}

template<typename T, typename U>
std::pair<int, int> Detector<T, U>::mapToPixelGrid(const vec3<T>& intersectionPoint) const {
    // Get required vectors and dimensions
    auto planarRect = dynamic_cast<PlanarRectangle<T, U>*>(this->surface.get());
    if (!planarRect) throw std::runtime_error("Surface is not a PlanarRectangle.");

    vec3<T> D_height = planarRect->getHeightDirection();
    vec3<T> D_width = planarRect->getNormal().cross(D_height).normalized();

    vec3<T> center = planarRect->getPosition(); 
    T width = planarRect->getWidth();
    T height = planarRect->getHeight();

    T u = (intersectionPoint - center).dot(D_width);
    T v = (intersectionPoint - center).dot(D_height);

    // Normalize local coordinates to [0, 1]
    T u_norm = (u + width / 2.0) / width;
    T v_norm = (v + height / 2.0) / height;

    // Map to pixel grid coordinates
    int xPixel = static_cast<int>(u_norm * pixelWidth);
    int yPixel = static_cast<int>(v_norm * pixelHeight);

    return std::make_pair(xPixel, yPixel);
}

template<typename T, typename U>
void Detector<T, U>::handleLight(Ray<T, U>& ray, const vec3<T>& intersectionPoint) {
    auto [columnIndex, rowIndex] = mapToPixelGrid(intersectionPoint);

    if (columnIndex >= 0 && columnIndex < pixelWidth && rowIndex >= 0 && rowIndex < pixelHeight) {
        this->pixelGrid(rowIndex, columnIndex) += ray.getIntensity(); 
    }
}
