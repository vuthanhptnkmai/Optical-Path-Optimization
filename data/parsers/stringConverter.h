#pragma once

#include <optional>
#include <string>
#include <sstream>
#include <iostream>

// Helper function for converting attribute to a value
template<typename T>
std::optional<T> convertAttribute(const std::string& attribute) {
    T value;
    std::istringstream iss(attribute);
    if (!(iss >> value)) {
        std::cerr << "Failed to convert attribute: " << attribute << std::endl;
        return std::nullopt;
    }
    return value;
}

// Generic template (not defined)
template<typename T>
std::optional<T> attributeToValue(const char* attribute);

// Specializations
template<>
std::optional<double> attributeToValue(const char* attribute) {
    return attribute ? convertAttribute<double>(attribute) : std::nullopt;
}

template<>
std::optional<float> attributeToValue(const char* attribute) {
    return attribute ? convertAttribute<float>(attribute) : std::nullopt;
}

template<>
std::optional<int> attributeToValue(const char* attribute) {
    return attribute ? convertAttribute<int>(attribute) : std::nullopt;
}

// Additional specializations can be added as needed
