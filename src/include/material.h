#ifndef MATERIAL_H
#define MATERIAL_H

#include "colorDBL.h"

class Material {
public:
    enum type { DIFFUSE, MIRROR, LIGHT };
    ColorDBL color;
    type materialType;

    Material(ColorDBL color, type MaterialType);
    Material();

    ColorDBL getColor() const;
    type getMaterialType() const;
};

#endif // MATERIAL_H
