#include "opticalSurfaces\planarCircle.h"
#include "opticalSurfaces\planarRectangle.h"
#include "opticalSurface.h"   // Assuming this is the path to your opticalSurface.h file

#include "ray.h"
#include "../utils/vec3.h"
#include <iostream>

int main() {
    using ValueType = double;  // Using double for this example
    using AttributeType = int; // Using int for attribute type for simplicity

    // Create a Ray
    Ray<ValueType, AttributeType> testRay(vec3<ValueType>(0, 0, 0), vec3<ValueType>(1, 1, 0), 650, 100);  // Providing all 4 arguments for the constructor

    // Test with PlanarCircle
    PlanarCircle<ValueType, AttributeType> circle(vec3<ValueType>(2, 2, 0), vec3<ValueType>(0, 0, 1), 2.0);
    // bool intersectsCircle = circle.intersects(testRay);
    // std::cout << "Ray intersects with circle: " << (intersectsCircle ? "Yes" : "No") << std::endl;

    // Test with PlanarRectangle (if it exists)
    // You should declare and define PlanarRectangle similarly to PlanarCircle. For now, I'll comment out the relevant lines.
    /*
    PlanarRectangle<ValueType, AttributeType> rect(vec3<ValueType>(5, 5, 0), vec3<ValueType>(0, 0, 1), 3.0, 4.0, vec3<ValueType>(1, 0, 0));
    bool intersectsRect = rect.intersects(testRay);
    std::cout << "Ray intersects with rectangle: " << (intersectsRect ? "Yes" : "No") << std::endl;
    */

    // Display the ray's attributes
    std::cout << "Ray Wavelength: " << testRay.getWavelength() << std::endl;
    std::cout << "Ray Intensity: " << testRay.getIntensity() << std::endl;

    return 0;
}
