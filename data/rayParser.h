#pragma once

#include <vector>
#include <iostream>
#include "stringConverter.h"
#include "../simulator/ray.h"
#include "../thirdParty/tinyxml2/tinyxml2.h"

template<typename T, typename U>
std::vector<Ray<T, U>> parseRayXML(const std::string& filename) {
    tinyxml2::XMLDocument doc;
    std::vector<Ray<T, U>> rays;

    // if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
    //     std::cerr << "Failed to load XML file: " << filename << std::endl;
    //     return rays; 
    // }

    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
    std::cerr << "Failed to load XML file: " << filename << ". Error: " << doc.ErrorStr() << std::endl;
    return rays; 
}


    tinyxml2::XMLElement* root = doc.FirstChildElement("rays");
    if (!root) {
        std::cerr << "Invalid XML format." << std::endl;
        return rays;
    }

    for (tinyxml2::XMLElement* elem = root->FirstChildElement("ray"); elem != nullptr; elem = elem->NextSiblingElement("ray")) {
        
        auto position = vec3<T>{
            attributeToValue<T>(elem->FirstChildElement("position")->Attribute("x")).value_or(0),
            attributeToValue<T>(elem->FirstChildElement("position")->Attribute("y")).value_or(0),
            attributeToValue<T>(elem->FirstChildElement("position")->Attribute("z")).value_or(0)
        };

        auto direction = vec3<T>{
            attributeToValue<T>(elem->FirstChildElement("direction")->Attribute("dx")).value_or(0),
            attributeToValue<T>(elem->FirstChildElement("direction")->Attribute("dy")).value_or(0),
            attributeToValue<T>(elem->FirstChildElement("direction")->Attribute("dz")).value_or(0)
        };

        U wavelength = attributeToValue<U>(elem->FirstChildElement("wavelength")->Attribute("value")).value_or(0);
        U intensity = attributeToValue<U>(elem->FirstChildElement("intensity")->Attribute("value")).value_or(0);

        rays.emplace_back(position, direction, wavelength, intensity);
    }

    return rays;
}
