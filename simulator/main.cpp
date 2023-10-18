#include "../data/rayParser.h"
#include "opticalSurfaces/planarCircle.h"
#include "ray.h"
#include <iostream>
#include <fstream>
#include <string>

void visualizeRayIntersections(const std::vector<Ray<double, float>>& rays, const Circle<double, float>& circle) {
    // Compute the End Points and Prepare the Data
    std::ofstream outputFile("intersections.dat"); // Write data to a file
    for (const auto& ray : rays) {
        if (circle.intersects(ray)) {
            vec3<double> intersection = circle.getIntersectionPoint(ray);
            outputFile << ray.position(0) << " " << ray.position(1) << " " << ray.position(2) << " "
                       << intersection(0) - ray.position(0) << " "
                       << intersection(1) - ray.position(1) << " "
                       << intersection(2) - ray.position(2) << "\n";
        }
    }
    outputFile.close();

    std::ofstream circleFile("circle.dat");
    for (double theta = 0; theta <= 2 * M_PI; theta += M_PI / 180.0) {
        double x = circle.getPosition().x() + circle.getRadius() * cos(theta);
        double y = circle.getPosition().y() + circle.getRadius() * sin(theta);
        double z = circle.getPosition().z(); // Assuming the circle lies on a constant z-plane
        circleFile << x << " " << y << " " << z << "\n";
    }
    circleFile.close();

    // Generate a Gnuplot script to visualize the ray paths
    std::ofstream plotFile("plot.gp");
    plotFile << "set terminal wxt size 1000,1000 enhanced font 'Verdana,10' persist\n";
    plotFile << "set view equal xyz\n";
    plotFile << "set xlabel 'x'\n";
    plotFile << "set ylabel 'y'\n";
    plotFile << "set zlabel 'z'\n";
    plotFile << "set xrange [0:15]\n";
    plotFile << "set yrange [0:15]\n";
    plotFile << "set zrange [0:15]\n";
    plotFile << "set autoscale x\n";
    plotFile << "set autoscale y\n";
    plotFile << "set autoscale z\n";
    plotFile << "set grid xtics mxtics\n";
    plotFile << "set mxtics 3\n";
    plotFile << "set grid ytics mytics\n";
    plotFile << "set mytics 3\n";
    plotFile << "set grid ztics mztics\n";
    plotFile << "set mztics 3\n";
    plotFile << "set grid\n";
    plotFile << "set xyplane 0\n";
    plotFile << "set style line 1 pointtype 7 pointsize 0.8\n";

    plotFile << "splot 'intersections.dat' using 1:2:3:4:5:6 with vectors head size 0,0 filled lc rgb 'red', \\\n";
    plotFile << "'circle.dat' using 1:2:3 with lines lc rgb 'blue'\n"; // Plot the circle in blue
    plotFile << "pause -1\n"; // Keep the window open
    plotFile.close();

    system("gnuplot plot.gp"); // Execute the Gnuplot script
}


int main() {

    std::vector<Ray<double, float>> rays = parseRayXML<double, float>("../data/rays1.xml");

    // Create a circular optical component
    Circle<double, float> circle(vec3<double>(7.5, 7.5, 7.5), vec3<double>(0, 0, 1), 10.0);

    // Visualize the ray intersections with the circle
    visualizeRayIntersections(rays, circle);
    
    return 0;
}