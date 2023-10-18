#include <iostream>

#include "opticalSurfaces/planarCircle.h"
#include "opticalSurfaces/planarRectangle.h"
#include "ray.h"
#include "../utils/vec3.h"


int main() {
    // Define some rays
    Ray<double, double> ray1(vec3<double>(0, 0, 0), vec3<double>(1, 1, 0), 650.0, 1.0); // Originating from origin, pointing diagonally in the XY plane
    Ray<double, double> ray2(vec3<double>(-5, 0, 0), vec3<double>(1, 0, 0), 650.0, 1.0); // Originating from left, pointing right along X axis
    Ray<double, double> ray3(vec3<double>(0, -5, 0), vec3<double>(0, 1, 0), 650.0, 1.0); // Originating from below, pointing up along Y axis

    // Define a circle with center at (2,2,0) in the XY plane with a diameter of 3 units
    Circle<double, double> circle(vec3<double>(2, 2, 0), vec3<double>(0, 0, 1), 3.0);

    // Define a rectangle with center at (2,2,0) in the XY plane, width 4 units, height 2 units, width direction along X axis
    Rectangle<double, double> rectangle(vec3<double>(2, 2, 0), vec3<double>(0, 0, 1), 4.0, 2.0, vec3<double>(1, 0, 0));

    // Check intersections with the circle
    std::cout << "Ray1 intersects with circle: " << (circle.intersects(ray1) ? "Yes" : "No") << std::endl;
    std::cout << "Ray2 intersects with circle: " << (circle.intersects(ray2) ? "Yes" : "No") << std::endl;
    std::cout << "Ray3 intersects with circle: " << (circle.intersects(ray3) ? "Yes" : "No") << std::endl;

    // Check intersections with the rectangle
    std::cout << "Ray1 intersects with rectangle: " << (rectangle.intersects(ray1) ? "Yes" : "No") << std::endl;
    std::cout << "Ray2 intersects with rectangle: " << (rectangle.intersects(ray2) ? "Yes" : "No") << std::endl;
    std::cout << "Ray3 intersects with rectangle: " << (rectangle.intersects(ray3) ? "Yes" : "No") << std::endl;

    return 0;
}
