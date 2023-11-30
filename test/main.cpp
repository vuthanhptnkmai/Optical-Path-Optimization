#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <memory>

// Mock class for OpticalComponent
class OpticalComponent {
public:
    // void generatePoints(const std::string& filename) {
    //     std::ofstream outFile(filename, std::ios::app);
    //     if (!outFile) {
    //         throw std::runtime_error("Failed to open " + filename + " for writing.");
    //     }

    //     // Write some mock data to the file
    //     outFile << "Points from " << typeid(*this).name() << std::endl;
    //     outFile.close();
    // }
    // Modify the generatePoints method to accept an ofstream reference
    void generatePoints(std::ofstream& outFile) {
        if (!outFile) {
            throw std::runtime_error("File stream is not open.");
        }
        // Write some mock data to the file
        outFile << "Points from " << typeid(*this).name() << std::endl;
    }
};

// Mock subclasses for Mirror, Lens, Filter, Detector
class Mirror : public OpticalComponent {};
class Lens : public OpticalComponent {};
class Filter : public OpticalComponent {};
class Detector : public OpticalComponent {};

int main() {
    try {
        // // Create instances of components
        // Mirror mirror;
        // Lens lens;
        // Filter filter;
        // Detector detector;

        // // Open a common file for all components
        // std::string commonFileName = "components.dat";
        // std::ofstream commonFile(commonFileName, std::ios::app); // Open in append mode
        // if (!commonFile) {
        //     throw std::runtime_error("Failed to open " + commonFileName + " for writing.");
        // }

        std::vector<std::unique_ptr<OpticalComponent>> components;

        // Add components to the vector
        components.push_back(std::make_unique<Mirror>());
        components.push_back(std::make_unique<Lens>());
        components.push_back(std::make_unique<Filter>());
        components.push_back(std::make_unique<Detector>());
        
        std::string commonFileName = "components.dat";
        std::ofstream commonFile(commonFileName, std::ios::out); // Open in write mode
        if (!commonFile) {
            throw std::runtime_error("Failed to open " + commonFileName + " for writing.");
        }

        // // Generate points for each component and write to the common file
        // mirror.generatePoints(commonFile);
        // lens.generatePoints(commonFile);
        // filter.generatePoints(commonFile);
        // detector.generatePoints(commonFile);

        // Loop through the vector and generate points for each component
        for (const auto& component : components) {
            component->generatePoints(commonFile);
        }
        
        // Close the common file after writing
        commonFile.close();

        std::cout << "Data generation completed successfully." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
