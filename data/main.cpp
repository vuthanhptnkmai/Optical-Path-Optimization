#include <iostream>
#include <fstream>
#include "photonParser.h"

#include <iostream>
#include <fstream>
#include "photonParser.h"

void visualizePhotons(const std::vector<Photon<double, float>>& photons) {
    // Compute the End Points and Prepare the Data
    std::ofstream outputFile("photons.dat"); // Write data to a file
    for (const auto& photon : photons) {
        outputFile << photon.position(0) << " " << photon.position(1) << " " << photon.position(2) << " "
                   << photon.direction(0) << " " << photon.direction(1) << " " << photon.direction(2) << "\n";
    }
    outputFile.close();

    // Generate a Gnuplot script to visualize the photon paths
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

    plotFile << "splot 'photons.dat' using 1:2:3:4:5:6 with vectors head size 0,0 filled lc rgb 'red'\n";
    plotFile << "pause -1\n"; // Keep the window open
    plotFile.close();

    system("gnuplot plot.gp"); // Execute the Gnuplot script
}


int main() {
    // Parse the XML file
    std::vector<Photon<double, float>> photons = parsePhotonsXML<double, float>("photons.xml");
    
    // Print the details of each photon
    for (const auto& photon : photons) {
        std::cout << "Photon Details:" << std::endl;
        std::cout << "Position: (" << photon.position(0) << ", " << photon.position(1) << ", " << photon.position(2) << ")" << std::endl;
        std::cout << "Direction: (" << photon.direction(0) << ", " << photon.direction(1) << ", " << photon.direction(2) << ")" << std::endl;
        std::cout << "Wavelength: " << photon.wavelength << std::endl;
        std::cout << "Intensity: " << photon.intensity << std::endl;
        std::cout << "----------------------" << std::endl;
    }

    // Visualize the photons
    visualizePhotons(photons);
    
    return 0;
}
