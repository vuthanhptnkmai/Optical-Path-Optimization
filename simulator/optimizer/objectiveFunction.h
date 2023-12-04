#pragma once

//#include <memory>
//#include <stdexcept>
// #include <utility>
#include "../../thirdParty/eigen-3.4.0/Eigen/Dense"

// used to be calculateVariance
template<typename U>
U sharpness(const Eigen::Matrix<U, Eigen::Dynamic, Eigen::Dynamic>& pixelGrid) {
    // Calculate the mean intensity of the image
    U mean = pixelGrid.mean();

    // Calculate the sum of the squared differences from the mean
    U sumSquaredDiffs = 0;
    for (int i = 0; i < pixelGrid.rows(); ++i) {
        for (int j = 0; j < pixelGrid.cols(); ++j) {
            U diff = pixelGrid(i, j) - mean;
            sumSquaredDiffs += diff * diff;
        }
    }

    // Calculate the variance
    U variance = sumSquaredDiffs / (pixelGrid.size() - 1);
    return variance;
}