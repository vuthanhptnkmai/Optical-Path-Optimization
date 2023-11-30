#pragma once

#include <utility>

#include "../ray.h"
#include "../opticalSurface.h"
#include "../../utils/vec3.h"

template<typename T, typename U>
class PlanarRectangle : public PlanarSurface<T, U> {
private:
    T width, height;
    vec3<T> widthDirection, heightDirection;  // widthDirection is provided, heightDirection is computed

public:
    PlanarRectangle(const vec3<T>& position, const vec3<T>& normal, T width, T height, const vec3<T>& widthDirection);

    T getWidth() const { return width; }
    T getHeight() const { return height; }
    const vec3<T>& getHeightDirection() const { return heightDirection; }
    const vec3<T>& getWidthDirection() const { return widthDirection; }

    std::pair<bool, vec3<T>> intersects(const Ray<T, U>& ray) const override;
    void generatePoints(const std::string& filename) const override;
};

// Definitions

template<typename T, typename U>
PlanarRectangle<T, U>::PlanarRectangle(const vec3<T>& position, const vec3<T>& normal, T width, T height, const vec3<T>& widthDirection)
    : PlanarSurface<T, U>(position, normal), width(width), height(height), 
      widthDirection(widthDirection.normalized()), 
      heightDirection(normal.cross(widthDirection).normalized()) {}
        
template<typename T, typename U>
std::pair<bool, vec3<T>> PlanarRectangle<T, U>::intersects(const Ray<T, U>& ray) const {
    vec3<T> intersectionPoint = this->getIntersectionPoint(ray);
    vec3<T> D = intersectionPoint - this->position;

    T u = D.dot(widthDirection);
    T v = D.dot(heightDirection);

    bool doesIntersect = (std::abs(u) <= width / 2.0) && (std::abs(v) <= height / 2.0);

    return {doesIntersect, intersectionPoint};
}

template<typename T, typename U>
void PlanarRectangle<T, U>::generatePoints(const std::string& filename) const {
    // Attempt to open the file for writing and handle any errors
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Failed to open " << filename << " for writing." << std::endl;
        return;
    }

    // Pre-calculate half width and height vectors
    vec3<T> halfWidthVec = widthDirection * (width / 2.0);
    vec3<T> halfHeightVec = heightDirection * (height / 2.0);

    // Define the corner points using vector arithmetic directly
    vec3<T> corners[4] = {
        this->position - halfWidthVec + halfHeightVec, // Top Left
        this->position + halfWidthVec + halfHeightVec, // Top Right
        this->position + halfWidthVec - halfHeightVec, // Bottom Right
        this->position - halfWidthVec - halfHeightVec  // Bottom Left
    };

    // Write the points to the file in a loop, closing the rectangle
    for (int i = 0; i < 4; ++i) {
        outFile << corners[i](0) << " " << corners[i](1) << " " << corners[i](2) << std::endl;
    }
    // Close the rectangle by connecting the last point to the first
    outFile << corners[0](0) << " " << corners[0](1) << " " << corners[0](2) << std::endl;

    // Close the file after writing
    outFile.close();
}
