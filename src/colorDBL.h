#include "glm/glm.hpp"
#include <algorithm>
#pragma once

class ColorDBL {
public:
    ColorDBL(double r = 0.0, double g = 0, double b = 0) : r(r), g(g), b(b) {}

    // Constructor accepting glm::dvec3
    ColorDBL(const glm::dvec3& color) : r(color.r), g(color.g), b(color.b) {}

    static ColorDBL red() { return ColorDBL(0.8, 0.1, 0.1); }
    static ColorDBL green() { return ColorDBL(0.0, 1.0, 0.0);}
    static ColorDBL blue() { return ColorDBL(0.0, 0.0, 1.0);}
    static ColorDBL white() { return ColorDBL(1.0, 1.0, 1.0); }
    static ColorDBL grey() { return ColorDBL(0.5, 0.5, 0.5);}

    // Clamp and convert to 8-bit RGB
    glm::dvec3 toRGB() const {
        return glm::dvec3(
            std::min(1.0, r) * 255,
            std::min(1.0, g) * 255,
            std::min(1.0, b) * 255
        );
    }

    // Add operator+=
    ColorDBL& operator+=(const ColorDBL& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    // Multiply operator*
    ColorDBL operator*(const ColorDBL& other) const {
        return ColorDBL(r * other.r, g * other.g, b * other.b);
    }

    // Multiply operator* with scalar
    ColorDBL operator*(double scalar) const {
        return ColorDBL(r * scalar, g * scalar, b * scalar);
    }

    // divide operator/
    ColorDBL operator/(const ColorDBL& other) const {
        return ColorDBL(r / other.r, g / other.g, b / other.b);
    }

    ColorDBL& operator/=(double scalar) {
        r /= scalar;

        g /= scalar;

        b /= scalar;

        return *this;
    }
    friend std::ostream& operator<<(std::ostream& os, const ColorDBL& color) {

        os << "(" << color.r << ", " << color.g << ", " << color.b << ")";

        return os;

    }

    double r, g, b;
};
