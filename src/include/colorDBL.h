#ifndef COLORDBL_H
#define COLORDBL_H

#include "glm/glm.hpp"
#include <algorithm>

class ColorDBL {
public:
    ColorDBL(double r = 0.0, double g = 0, double b = 0);

    // Constructor accepting glm::dvec3
    ColorDBL(const glm::dvec3& color);

    static ColorDBL red();
    static ColorDBL green();
    static ColorDBL blue();
    static ColorDBL white();
    static ColorDBL grey();

    glm::dvec3 toRGB() const;

    ColorDBL& operator+=(const ColorDBL& other);
    ColorDBL operator*(const ColorDBL& other) const;
    ColorDBL operator+(const ColorDBL& other) const;
    // double * ColorDBL
    ColorDBL operator*(double scalar) const;
    ColorDBL operator/(const ColorDBL& other) const;
    ColorDBL& operator/=(double scalar);
    double r, g, b;
};

#endif // COLORDBL_H
