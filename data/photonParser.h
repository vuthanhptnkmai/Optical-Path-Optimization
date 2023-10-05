#pragma once

#include <vector>
#include <iostream>
#include "stringConverter.h"
#include "../simulator/photon.h"
#include "../thirdParty/tinyxml2/tinyxml2.h"

template<typename T, typename U>
std::vector<Photon<T, U>> parsePhotonsXML(const std::string& filename) {
    tinyxml2::XMLDocument doc;
    std::vector<Photon<T, U>> photons;

    if (doc.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load XML file: " << filename << std::endl;
        return photons; 
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("photons");
    if (!root) {
        std::cerr << "Invalid XML format." << std::endl;
        return photons;
    }

    for (tinyxml2::XMLElement* elem = root->FirstChildElement("photon"); elem != nullptr; elem = elem->NextSiblingElement("photon")) {
        
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

        photons.emplace_back(position, direction, wavelength, intensity);
    }

    return photons;
}
