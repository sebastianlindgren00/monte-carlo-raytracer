#include "glm/glm.hpp"
#include <algorithm>

class ColorDBL {
public:
    ColorDBL(float r = 0, float g = 0, float b = 0) : r(r), g(g), b(b) {}

    // Constructor accepting glm::vec3
    ColorDBL(const glm::vec3& color) : r(color.r), g(color.g), b(color.b) {}

    // Clamp and convert to 8-bit RGB
    glm::vec3 toRGB() const {
        return glm::vec3(
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

    float r, g, b;
};
