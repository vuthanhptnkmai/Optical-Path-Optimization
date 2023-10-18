#include <iostream>
#include <memory>

#include "opticalcomponent.h"
#include "../utils/vec3.h"
#include "shape.h"

int main() {
    // Create a unique_ptr of type OpticalComponent pointing to a Mirror object
    std::unique_ptr<OpticalComponent<float, float>> oc = std::make_unique<Mirror<float, float>>(vec3<float>(0.0f, 0.0f, 0.0f), vec3<float>(0.0f, 1.0f, 0.0f), std::make_unique<Rectangle<float>>(5.0f, 5.0f));

    // Display the position of the optical component
    vec3<float> position = oc->getPosition();
    std::cout << "Position of the optical component: (" << position(0) << ", " << position(1) << ", " << position(2) << ")" << std::endl;

    // Change the optical component to a Filter object
    oc = std::make_unique<Filter<float, float>>(vec3<float>(1.0f, 1.0f, 1.0f), vec3<float>(0.0f, 1.0f, 0.0f), std::make_unique<Circle<float>>(5.0f), "Red", 650.0f);

    // Display the position of the new optical component
    position = oc->getPosition();
    std::cout << "Position of the new optical component: (" << position(0) << ", " << position(1) << ", " << position(2) << ")" << std::endl;

    return 0;
}