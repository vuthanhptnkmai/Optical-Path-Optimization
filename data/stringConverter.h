#pragma once

#include <optional>
#include <string>

// Generic template (not defined)
template<typename T>
std::optional<T> attributeToValue(const char* attribute);

// Specialization for double
template<>
std::optional<double> attributeToValue(const char* attribute) {
    if (!attribute) {
        return std::nullopt;
    }
    try {
        return std::stod(attribute);
    } catch(const std::exception& e) {
        return std::nullopt;
    }
}

// Specialization for float
template<>
std::optional<float> attributeToValue(const char* attribute) {
    if (!attribute) {
        return std::nullopt;
    }
    try {
        return std::stof(attribute);
    } catch(const std::exception& e) {
        return std::nullopt;
    }
}

// Specialization for int
template<>
std::optional<int> attributeToValue(const char* attribute) {
    if (!attribute) {
        return std::nullopt;
    }
    try {
        return std::stoi(attribute);
    } catch(const std::exception& e) {
        return std::nullopt;
    }
}
