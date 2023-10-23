#pragma once

#include <memory>
#include <vector>

#include "../opticalComponent.h"
#include "../opticalSurface.h"

#include "../opticalSurfaces/PlanarCircle.h"
#include "../opticalComponents/mirror.h"
#include "../opticalComponents/lens.h"

#include "../../thirdParty/eigen-3.4.0/Eigen/Dense"
#include "../../utils/vec3.h"
#include "../ray.h"  

template <typename T, typename U>
std::unique_ptr<std::vector<std::unique_ptr<OpticalComponent<T, U>>>> readComponentsFromFile() {
    auto components = std::make_unique<std::vector<std::unique_ptr<OpticalComponent<T, U>>>>();

    vec3<T> position1(1.0, 1.0, 1.0);
    vec3<T> normal1(0.0, 0.0, 1.0);
    T diameter1 = 2.0;
    auto circle1 = std::make_unique<PlanarCircle<T, U>>(position1, normal1, diameter1);
    auto mirror = std::make_unique<PlanarMirror<T, U>>(circle1);

    vec3<T> position2(1.0, 1.0, 1.0);
    vec3<T> normal2(0.0, 0.0, 1.0);
    T diameter2 = 2.0;
    auto circle2 = std::make_unique<PlanarCircle<T, U>>(position2, normal2, diameter2);
    auto convex_lens = std::make_unique<ThinLens<T, U>>(circle2, -0.5);

    vec3<T> position3(1.0, 1.0, 1.0);
    vec3<T> normal3(0.0, 0.0, 1.0);
    T diameter3 = 2.0;
    auto circle3 = std::make_unique<PlanarCircle<T, U>>(position3, normal3, diameter3);
    auto concave_lens = std::make_unique<ThinLens<T, U>>(circle3, -0.5);
    
    components->push_back(std::move(mirror));
    components->push_back(std::move(convex_lens));
    components->push_back(std::move(concave_lens));

    return components;
}
