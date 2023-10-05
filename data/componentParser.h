#pragma once

#include <vector>
#include <iostream>
#include "../simulator/opticalComponent.h"
#include "thirdParty/tinyxml2/tinyxml2.h"

template<typename T, typename U>
std::vector<OpticalComponent<T, U>> parseComponentsXML(const std::string& filename);

#include "componentParser.h"

template<typename T, typename U>
std::vector<OpticalComponent<T, U>> parseComponentsXML(const std::string& filename) {
    tinyxml2::XMLDocument doc;
    std::vector<OpticalComponent<T, U>> components;

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
        // Parse the component details here and add to the components vector
        // You'll need to define the OpticalComponent class and its constructors to match the XML structure
    }

    return components;
}

// Explicit instantiation for common types if needed
template std::vector<OpticalComponent<double, double>> parseComponentsXML<double, double>(const std::string& filename);
