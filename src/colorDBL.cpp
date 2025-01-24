#include "include/colorDBL.h"

ColorDBL::ColorDBL(double r, double g, double b) : r(r), g(g), b(b) {}
ColorDBL::ColorDBL(const glm::dvec3& color) : r(color.r), g(color.g), b(color.b) {}

ColorDBL ColorDBL::red() { return ColorDBL(0.8, 0.1, 0.1); }
ColorDBL ColorDBL::green() { return ColorDBL(0.0, 1.0, 0.0);}
ColorDBL ColorDBL::blue() { return ColorDBL(0.0, 0.0, 1.0);}
ColorDBL ColorDBL::white() { return ColorDBL(1.0, 1.0, 1.0); }
ColorDBL ColorDBL::grey() { return ColorDBL(0.5, 0.5, 0.5);}

glm::dvec3 ColorDBL::toRGB() const {
    return glm::dvec3(
        std::min(1.0, r) * 255,
        std::min(1.0, g) * 255,
        std::min(1.0, b) * 255
    );
}

// Operators
ColorDBL& ColorDBL::operator+=(const ColorDBL& other) {
    r += other.r;
    g += other.g;
    b += other.b;
    return *this;
}

ColorDBL ColorDBL::operator+(const ColorDBL& other) const {
    return ColorDBL(r + other.r, g + other.g, b + other.b);
}

ColorDBL ColorDBL::operator*(const ColorDBL& other) const {
    return ColorDBL(r * other.r, g * other.g, b * other.b);
}

ColorDBL ColorDBL::operator*(double scalar) const {
    return ColorDBL(r * scalar, g * scalar, b * scalar);
}

ColorDBL ColorDBL::operator/(const ColorDBL& other) const {
    return ColorDBL(r / other.r, g / other.g, b / other.b);
}

ColorDBL& ColorDBL::operator/=(double scalar) {
    r /= scalar;
    g /= scalar;
    b /= scalar;
    return *this;
}