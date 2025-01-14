#include "glm/glm.hpp"
#include <algorithm>

class ColorDBL {
public:
    ColorDBL(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}

    // Constructor accepting glm::dvec3
    ColorDBL(const glm::dvec3& color) : r(color.r), g(color.g), b(color.b) {}

    // Clamp and convert to 8-bit RGB
    glm::dvec3 toRGB() const {
        return glm::dvec3(
            std::min(1.0f, r) * 255,
            std::min(1.0f, g) * 255,
            std::min(1.0f, b) * 255
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

    float r, g, b;
};
