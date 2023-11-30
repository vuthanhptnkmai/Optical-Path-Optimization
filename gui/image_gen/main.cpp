#include <vector>
#include <string>
#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Function to save an image as a PNG file
void saveImageAsPNG(const std::string& filename, const std::vector<std::vector<float>>& pixelData, int width, int height) {
    std::vector<uint8_t> image;
    for (const auto& row : pixelData) {
        for (float value : row) {
            image.push_back(static_cast<uint8_t>(value * 255));
        }
    }

    stbi_write_png(filename.c_str(), width, height, 1, image.data(), width);
}

// Main function to test the image saving functionality
int main() {
    try {
        // Example: create a simple 512x512 grayscale image
        int width = 512, height = 512;
        std::vector<std::vector<float>> pixelData(height, std::vector<float>(width, 0.5f)); // Fill with a constant value (50% gray)

        // Saving the image
        std::string filename = "test_image.png";
        saveImageAsPNG(filename, pixelData, width, height);

        std::cout << "Image saved successfully: " << filename << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
