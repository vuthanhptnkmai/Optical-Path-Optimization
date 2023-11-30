#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <memory>
#include "ray.h"
#include "../utils/vec3.h"

#include "opticalComponent.h"
#include "opticalSurfaces/PlanarRectangle.h"
#include "opticalComponents/Detector.h"

#include "../thirdParty/eigen-3.4.0/Eigen/Geometry"
#include "../thirdParty/eigen-3.4.0/Eigen/Dense"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../thirdParty/stb_image_write/stb_image_write.h"

using T = double;
using U = float;

// Function to save an image as a PNG file
void saveImageAsPNG(const std::string& filename, const Eigen::MatrixXf& pixelGrid) {
    int width = pixelGrid.cols(), height = pixelGrid.rows();
    std::vector<uint8_t> image(width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            image[y * width + x] = static_cast<uint8_t>(pixelGrid(y, x) * 255);
        }
    }

    stbi_write_png(filename.c_str(), width, height, 1, image.data(), width);
}

int main() {
    try {
        // Create four rays with random wavelengths
        std::vector<Ray<T, U>> rays = {
            Ray<T, U>(vec3<T>(0, 0, 0), vec3<T>(0, 0, 1), 456, 1.0f),
            Ray<T, U>(vec3<T>(0, 1, 0), vec3<T>(0, 0, 1), 475, 1.0f),
            Ray<T, U>(vec3<T>(1, 1, 0), vec3<T>(0, 0, 1), 493, 1.0f),
            Ray<T, U>(vec3<T>(1, 0, 0), vec3<T>(0, 0, 1), 480, 1.0f)
        };

        // Create a Detector instance as a pointer
        auto detectorSurface = std::make_unique<PlanarRectangle<T, U>>(vec3<T>(0.75, 0.75, 1), vec3<T>(0, 0, 1), 1.2, 3, vec3<T>(1, 0, 0));
        auto detector = std::make_unique<Detector<T, U>>(std::move(detectorSurface), 512, 256);

        detector->getSurfacePtr()->generatePoints("component1.dat");

        // Open a file to write ray data
        std::ofstream outFile("rays.dat");
        if (!outFile) {
            throw std::runtime_error("Failed to open rays.dat for writing.");
        }

        for (auto& ray : rays) { 
            outFile << ray.position(0) << " " << ray.position(1) << " " << ray.position(2) << std::endl;

            auto [doesIntersect, intersectionPoint] = detector->getSurfacePtr()->intersects(ray);
            outFile << intersectionPoint(0) << " " << intersectionPoint(1) << " " << intersectionPoint(2) << "\n\n" << std::endl;
            
            if (doesIntersect) {
                detector->handleLight(ray, intersectionPoint);
                detector->getSurfacePtr()->generatePoints("component2.dat");
            } else {
                outFile << "\n\n" << std::endl; 
                continue; 
            }
        }

        // Save the final state of the detector's pixel grid as a PNG image
        std::string filename = "detector_output.png";
        saveImageAsPNG(filename, detector->getPixelGrid());

        std::cout << "Detector output saved as: " << filename << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
