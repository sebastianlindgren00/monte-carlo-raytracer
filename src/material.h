#include "colorDBL.h"
#pragma once
class Material {
    public:
    enum type {DIFFUSE, MIRROR, LIGHT};
    ColorDBL color;
    type MaterialType;

    Material(ColorDBL color, type MaterialType) : color(color), MaterialType(MaterialType) {}

    Material() : color(ColorDBL(1.0, 0.0, 0.0)), MaterialType(DIFFUSE) {}

    ColorDBL getColor() const {
        return color;
    }

    type getMaterialType() {
        return MaterialType;
    }
};