#include <fstream>
#include <iostream>
#include <stdexcept>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "utils/vec3.h"

#include "simulator/simulator.h"
#include "data/parsers/parsers.h"

#include "thirdParty/eigen-3.4.0/Eigen/Geometry"
#include "thirdParty/eigen-3.4.0/Eigen/Dense"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "thirdParty/stb_image_write/stb_image_write.h"

using T = double;
using U = float;

// Function to save an image as a PNG file
void saveImageAsPNG(const std::string& filename, const Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>& pixelGrid) {
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
        // Read rays and components from files
        auto rays = parseRaysDat<double, float>("data/rays100.dat");
        auto components = parseComponentsXML<double, float>("data/components.xml");

        ThinLens<T, U>* lensPtr = dynamic_cast<ThinLens<T, U>*>(components[1].get());
        Detector<T, U>* detectorPtr = dynamic_cast<Detector<T, U>*>(components.back().get());
        
        // Define the range and step size for autofocus
        T startZ = 2.5;  // Start position of z-axis movement
        T endZ = 3.5;     // End position of z-axis movement
        T stepZ = 0.01;    // Step size for each movement along the z-axis
        // autoFocus(rays, components, lensPtr, detectorPtr, startZ, endZ, stepZ, false);
    
        rayTracingWithLogging(rays, components, "visualization/rays.dat", "visualization/components.dat");

        // Save detector's pixel grid as a PNG image
        std::string filename = "visualization/image.png";
        if (detectorPtr) {
            // Retrieve the pixel grid from the detector
            auto pixelGrid = detectorPtr->getPixelGrid();
            
            saveImageAsPNG(filename, detectorPtr->getPixelGrid());
            std::cout << "Detector output saved as: " << filename << std::endl;
            
            // Calculate the sharpness using the variance of the pixel intensities
            auto sharpness = sharpnessVariance(pixelGrid);
            std::cout << "Sharpness (Variance): " << sharpness << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}