#pragma once

template<typename T>
class Shape {
public:
    virtual ~Shape() = default; // the compiler will generate the default implementation for the destructor
};

template<typename T>
class Rectangle : public Shape<T> {
protected:
    T width, height;
public:
    Rectangle(T width, T height);
};

template<typename T>
class Circle : public Shape<T> {
protected:
    T diameter;
public:
    Circle(T diameter);
};

// Definitions

template<typename T>
Rectangle<T>::Rectangle(T width, T height) : width(width), height(height) {}

template<typename T>
Circle<T>::Circle(T diameter) : diameter(diameter) {}