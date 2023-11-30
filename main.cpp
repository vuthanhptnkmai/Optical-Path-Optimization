// main.cpp
#include <iostream>
#include <fstream>
#include <Eigen/Geometry> // Make sure to include this header for Eigen::AngleAxis

// Assuming vec3 is defined somewhere, or using Eigen's Vector3 directly
template<typename T>
using vec3 = Eigen::Matrix<T, 3, 1>;

template<typename T, typename U>
class PlanarCircle {
private:
    vec3<T> position;
    vec3<T> normal;
    U radius;

public:
    PlanarCircle(const vec3<T>& position, const vec3<T>& normal, U radius)
        : position(position), normal(normal), radius(radius) {}

    void generatePoints(const std::string& filename) const {
        int numPoints = static_cast<int>(radius * 2 * M_PI * 5);

        vec3<T> defaultNormal(0, 1, 0);
        vec3<T> rotationAxis = defaultNormal.cross(normal).normalized();
        T angle = std::acos(defaultNormal.dot(normal.normalized()));

        Eigen::AngleAxis<T> rotation(angle, rotationAxis);

        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Failed to open " << filename << " for writing." << std::endl;
            return;
        }

        for (int i = 0; i <= numPoints; ++i) {
            T t = i * 2 * M_PI / numPoints;
            vec3<T> point(radius * std::cos(t), 0, radius * std::sin(t));

            vec3<T> rotatedPoint = rotation * point;
            vec3<T> finalPoint = rotatedPoint + position;

            outFile << finalPoint(0) << " " << finalPoint(1) << " " << finalPoint(2) << std::endl;
        }
        outFile << "\n\n" << std::endl;
        outFile.close();
    }
};

int main() {
    // Example usage
    vec3<double> circlePos(0, 0, 0);
    vec3<double> circleNormal(0, 0, 1); // Assuming the circle lies in the XY plane
    double circleRadius = 5.0;

    PlanarCircle<double, double> circle(circlePos, circleNormal, circleRadius);
    circle.generatePoints("circle_points.txt");

    std::cout << "Generated circle points." << std::endl;
    return 0;
}
