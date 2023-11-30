    #include <fstream>
    #include <iostream>
    #include <stdexcept>

    #include <vector>
    #include <memory>

    #include "ray.h"
    #include "../utils/vec3.h"

    #include "opticalComponent.h"

    #include "opticalSurfaces/PlanarCircle.h"
    #include "opticalComponents/Mirror.h"
    #include "opticalComponents/Lens.h"
    #include "opticalComponents/Filter.h"

    #include "opticalSurfaces/PlanarRectangle.h"
    #include "opticalComponents/Detector.h"

    #include "utils/readRaysFromFile.h"


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
            auto rays = readRaysFromFile("utils/ray_input.dat");

            // Create a PlanarMirror
            vec3<T> mirrorPosition(0.0, 0.0, 0.0);
            vec3<T> mirrorNormal(0, 1.0, 1.0);
            T mirrorDiameter = 0.5;
            auto mirrorSurface = std::make_unique<PlanarCircle<T, U>>(mirrorPosition, mirrorNormal, mirrorDiameter);
            auto mirror = std::make_unique<PlanarMirror<T, U>>(std::move(mirrorSurface));

            // Create a ThinLens
            vec3<T> lensPosition(0.0, 0.0, 1.0);
            vec3<T> lensNormal(0.0, 0.0, 1.0);
            T lensDiameter = 1.5;
            U lensFocalLength = 1.0;
            auto lensSurface = std::make_unique<PlanarCircle<T, U>>(lensPosition, lensNormal, lensDiameter);
            auto lens = std::make_unique<ThinLens<T, U>>(std::move(lensSurface), lensFocalLength);

            // Create a Filter
            vec3<T> filterPosition(0.0, 0.0, 2.0);
            vec3<T> filterNormal(0.0, 0.0, 1.0);
            T filterDiameter = 2.0;
            U minWavelength = 450.0, maxWavelength = 700.0;
            auto filterSurface = std::make_unique<PlanarCircle<T, U>>(filterPosition, filterNormal, filterDiameter);
            auto filter = std::make_unique<Filter<T, U>>(std::move(filterSurface), minWavelength, maxWavelength);

            // Create a Detector
            vec3<T> detectorPosition(0.0, 0.0, 3.0);
            vec3<T> detectorNormal(0.0, 0.0, 1.0);
            vec3<T> detectorHeightDirection(0.0, 1.0, 0.0);
            T detectorWidth = 1.0, detectorHeight = 1.0;
            int pixelWidth = 512, pixelHeight = 512;
            auto detectorSurface = std::make_unique<PlanarRectangle<T, U>>(detectorPosition, detectorNormal, detectorWidth, detectorHeight, detectorHeightDirection);
            auto detector = std::make_unique<Detector<T, U>>(std::move(detectorSurface), pixelWidth, pixelHeight);

            // Store components in a vector of unique pointers to OpticalComponent
            std::vector<std::unique_ptr<OpticalComponent<T, U>>> components;
            components.push_back(std::move(mirror));
            components.push_back(std::move(lens));
            components.push_back(std::move(filter));
            components.push_back(std::move(detector));

            // Open a file to write component data
            std::string componentsFileName = "components.dat";
            std::ofstream componentsFile(componentsFileName, std::ios::out);
            if (!componentsFile) {
                throw std::runtime_error("Failed to open " + componentsFileName + " for writing.");
            }

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

            componentsFile.close();
            std::cout << "Component data generation completed successfully." << std::endl;

            raysFile.close();
            std::cout << "Ray data generation completed successfully." << std::endl;

            // Save detector's pixel grid as a PNG image
            std::string filename = "detector_output.png";
            auto detectorPtr = dynamic_cast<Detector<T, U>*>(components.back().get());
            if (detectorPtr) {
                saveImageAsPNG(filename, detectorPtr->getPixelGrid());
                std::cout << "Detector output saved as: " << filename << std::endl;
            }

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }

        return 0;
    }


    //         for (auto& ray : *rays) { 
    //             outFile << ray.position(0) << " " << ray.position(1) << " " << ray.position(2) << std::endl;

    //             auto [doesIntersect, intersectionPoint] = mirror->getSurfacePtr()->intersects(ray);
    //             outFile << intersectionPoint(0) << " " << intersectionPoint(1) << " " << intersectionPoint(2) << std::endl;
                
    //             if (doesIntersect) {
    //                 mirror->handleLight(ray, intersectionPoint);
    //                 outFile << ray.position(0) + ray.direction(0) << " " << ray.position(1) + ray.direction(1) << " " << ray.position(2) + ray.direction(2) << "\n\n" << std::endl;
    //             } else {
    //                 outFile << "\n\n" << std::endl; 
    //                 continue; 
    //             }
    //         }

    //         // Save detector's pixel grid as a PNG image
    //         std::string filename = "detector_output.png";
    //         auto detectorPtr = dynamic_cast<Detector<T, U>*>(components.back().get());
    //         if (detectorPtr) {
    //             saveImageAsPNG(filename, detectorPtr->getPixelGrid());
    //             std::cout << "Detector output saved as: " << filename << std::endl;
    //         }

    //     } catch (const std::exception& e) {
    //         std::cerr << "Error: " << e.what() << std::endl;
    //         return 1;
    //     }

    //     return 0;
    // }