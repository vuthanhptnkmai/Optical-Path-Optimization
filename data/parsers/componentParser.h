#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <memory>

#include "stringConverter.h"
#include "../../simulator/simulator.h"
#include "../../thirdParty/tinyxml2/tinyxml2.h"

template<typename T, typename U>
std::vector<std::unique_ptr<OpticalComponent<T, U>>> parseComponentsXML(const std::string& filename) {
    tinyxml2::XMLDocument doc;
    std::vector<std::unique_ptr<OpticalComponent<T, U>>> components;

    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load XML file: " << filename << std::endl;
        return components;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("OpticalComponents");
    if (!root) {
        std::cerr << "Invalid XML format." << std::endl;
        return components;
    } 

    for (tinyxml2::XMLElement* elem = root->FirstChildElement("Component"); elem != nullptr; elem = elem->NextSiblingElement("Component")) {
        std::string name = elem->Attribute("name");
        
        // Extract common attributes
        auto surfaceElem = elem->FirstChildElement("Surface");
        auto positionElem = elem->FirstChildElement("Position");
        auto normalElem = elem->FirstChildElement("Normal");
        // vec3<T> position(positionElem->FloatAttribute("x"), positionElem->FloatAttribute("y"), positionElem->FloatAttribute("z"));
        // vec3<T> normal(normalElem->FloatAttribute("dx"), normalElem->FloatAttribute("dy"), normalElem->FloatAttribute("dz"));
        vec3<T> position(
            attributeToValue<T>(positionElem->Attribute("x")).value_or(T()),
            attributeToValue<T>(positionElem->Attribute("y")).value_or(T()),
            attributeToValue<T>(positionElem->Attribute("z")).value_or(T())
        );
        vec3<T> normal(
            attributeToValue<T>(normalElem->Attribute("dx")).value_or(T()),
            attributeToValue<T>(normalElem->Attribute("dy")).value_or(T()),
            attributeToValue<T>(normalElem->Attribute("dz")).value_or(T())
        );

        if (name == "Mirror") {
            T diameter = static_cast<T>(surfaceElem->FloatAttribute("diameter"));
            auto mirrorSurface = std::make_unique<PlanarCircle<T, U>>(position, normal, diameter);
            auto mirror = std::make_unique<PlanarMirror<T, U>>(std::move(mirrorSurface));
            components.push_back(std::move(mirror));
        } 
        else if (name == "Lens") {
            T diameter = static_cast<T>(surfaceElem->FloatAttribute("diameter"));
            auto focalLengthElem = elem->FirstChildElement("FocalLength");
            U focalLength = static_cast<U>(focalLengthElem->FloatAttribute("value"));

            auto lensSurface = std::make_unique<PlanarCircle<T, U>>(position, normal, diameter);
            auto lens = std::make_unique<ThinLens<T, U>>(std::move(lensSurface), focalLength);
            components.push_back(std::move(lens));
        } 
        else if (name == "Filter") {
            T diameter = static_cast<T>(surfaceElem->FloatAttribute("diameter"));
            auto wavelengthRangeElem = elem->FirstChildElement("WavelengthRange");
            U minWavelength = static_cast<U>(wavelengthRangeElem->FloatAttribute("min"));
            U maxWavelength = static_cast<U>(wavelengthRangeElem->FloatAttribute("max"));

            auto filterSurface = std::make_unique<PlanarCircle<T, U>>(position, normal, diameter);
            auto filter = std::make_unique<Filter<T, U>>(std::move(filterSurface), minWavelength, maxWavelength);
            components.push_back(std::move(filter));
        }
        else if (name == "Detector") {
            T width = static_cast<T>(surfaceElem->FloatAttribute("width"));
            T height = static_cast<T>(surfaceElem->FloatAttribute("height"));
            auto pixelElem = elem->FirstChildElement("Pixels");
            int pixelWidth = pixelElem->IntAttribute("width");
            int pixelHeight = pixelElem->IntAttribute("height");
            auto heightDirElem = elem->FirstChildElement("HeightDirection");
            vec3<T> heightDirection(heightDirElem->FloatAttribute("hx"), heightDirElem->FloatAttribute("hy"), heightDirElem->FloatAttribute("hz"));
            
            auto detectorSurface = std::make_unique<PlanarRectangle<T, U>>(position, normal, width, height, heightDirection);
            auto detector = std::make_unique<Detector<T, U>>(std::move(detectorSurface), pixelWidth, pixelHeight);
            components.push_back(std::move(detector));
        }
    }

    return components;
}