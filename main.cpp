#include <fstream>
#include <iostream>
#include <stdexcept>

#include <vector>
#include <memory>

#include "simulator/simulator.h"

#include "utils/vec3.h"
#include "../data/parsers/parsers.h"

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
        // Read rays from a file
        //auto rays = readRaysFromFile("utils/ray_input.dat");
        auto rays = parseRaysDat<double, float>("../data/rays100.dat");
        auto components = parseComponentsXML<double, float>("../data/components.xml");

        // for (auto& component1 : components1) {
        //     for (auto& component : components) {
        //         auto lensPtr = dynamic_cast<ThinLens<T, U>*>(components1[1].get());
        //         auto lensPtr1 = dynamic_cast<ThinLens<T, U>*>(components[1].get());
        //         //if(lensPtr->getSurfacePtr()->getNormal() == lensPtr->getSurfacePtr()->getNormal()) {
        //         if(lensPtr->focalLength == lensPtr1->focalLength) {
        //             std::cout << "ok" << std::endl;
        //         } else {
        //             std::cout << "not ok" << std::endl;
        //         }
        //     }
        // }

        ThinLens<T, U>* lensPtr = dynamic_cast<ThinLens<T, U>*>(components[1].get());
        Detector<T, U>* detectorPtr = dynamic_cast<Detector<T, U>*>(components.back().get());
        
        // Open a file to write component data
        std::string componentsFileName = "components.dat";
        std::ofstream componentsFile(componentsFileName, std::ios::out);
        if (!componentsFile) {
            throw std::runtime_error("Failed to open " + componentsFileName + " for writing.");
        }

        // // Define the range and step size for autofocus
        // T startZ = 2.5;  // Start position of z-axis movement
        // T endZ = 3.5;     // End position of z-axis movement
        // T stepZ = 0.5;    // Step size for each movement along the z-axis

        // // Call the autofocus function with pointers
        // // T bestLensZ = autoFocus(rays, components, startZ, endZ, stepZ);
        // // std::cout << "Autofocus completed. Best focus position: " << bestLensZ << std::endl;
        // T bestZ = startZ;
        // U bestSharpness = -std::numeric_limits<U>::max();
    
      
        // std::cout << "Start dynamic casting lensPtr..." << std::endl;
        // std::cout << "Finish dynamic casting lensPtr!!" << std::endl;

        // // auto& lens = components[1]; // Assuming the lens is at index 1
        // for (T z = startZ; z <= endZ; z += stepZ) {

        //     // Get a non-const pointer to the lens's surface
        //     auto lensSurface = lensPtr->getSurfacePtr();
        //     if (lensSurface) {
        //         vec3<T> newPos = lensSurface->getPosition();
        //         newPos(2) = z; // Update the z-coordinate
        //         // std::cout << "Lens Position - X: " << newPos(0) << ", Y: " << newPos(1) << ", Z: " << newPos(2) << std::endl;
        //         lensSurface->setPosition(newPos); // Call setPosition on the non-const pointer
        //     }
            
        //     // std::cout << lensPtr->getSurfacePtr()->getPosition()[2] << " " << dynamic_cast<ThinLens<T, U>*>(components[1].get())->getSurfacePtr()->getPosition()[2] << std::endl;
        //     // std::cout << lensPtr << " " << dynamic_cast<ThinLens<T, U>*>(components[1].get()) << std::endl;
        //     // Capture the image with the current lens position
        //     captureImage(rays, components);

        //     // Calculate the sharpness of the current image
        //     U currentSharpness = calculateSharpness(detectorPtr->getPixelGrid());

        //     std::cout << currentSharpness << std::endl;
        //     // If the current sharpness is the best so far, update bestZ and bestSharpness
        //     if (currentSharpness > bestSharpness) {
        //         bestSharpness = currentSharpness;
        //         bestZ = z;
        //     }
            
        //     rays = readRaysFromFile("utils/ray_input.dat");
        //     detectorPtr->resetPixelGrid();
        // }

        // std::cout << "Bestz = " << bestZ << std::endl;

        // Loop through the vector and generate points for each component's surface
        for (const auto& component : components) {
            auto surface = component->getSurfacePtr();
            if (surface) {
                surface->generatePoints(componentsFile);
            }
        }

        // Open a file to write ray data
        std::string raysFileName = "rays.dat";
        std::ofstream raysFile(raysFileName, std::ios::out);
        if (!raysFile) {
            throw std::runtime_error("Failed to open " + raysFileName + " for writing.");
        }

        for (auto& ray : *rays) { 
            raysFile << ray.position(0) << " " << ray.position(1) << " " << ray.position(2) << std::endl;

            for (auto& component : components) {
                
                //raysFile << ray.position(0) << " " << ray.position(1) << " " << ray.position(2) << std::endl;

                auto [doesIntersect, intersectionPoint] = component->getSurfacePtr()->intersects(ray);
                raysFile << intersectionPoint(0) << " " << intersectionPoint(1) << " " << intersectionPoint(2) << std::endl;
                
                if (doesIntersect) {
                    component->handleLight(ray, intersectionPoint);
                    // // Assuming you still need to generate points here
                    // component->getSurfacePtr()->generatePoints(componentsFile);
                } else {
                    break; // Skip remaining components if no intersection
                }
            }

            raysFile << "\n\n" << std::endl; 
        }

        // captureImage(rays, components);

        componentsFile.close();
        std::cout << "Component data generation completed successfully." << std::endl;

        raysFile.close();
        std::cout << "Ray data generation completed successfully." << std::endl;

        // Save detector's pixel grid as a PNG image
        std::string filename = "detector_output.png";
        // auto detectorPtr = dynamic_cast<Detector<T, U>*>(components.back().get());
        if (detectorPtr) {
            // Retrieve the pixel grid from the detector
            auto pixelGrid = detectorPtr->getPixelGrid();
            
            saveImageAsPNG(filename, detectorPtr->getPixelGrid());
            std::cout << "Detector output saved as: " << filename << std::endl;
            
            // Calculate the sharpness using the variance of the pixel intensities
            auto sharpness = calculateSharpness(pixelGrid);
            std::cout << "Sharpness (Variance): " << sharpness << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}