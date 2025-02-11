#include "material.h"

Material::Material(ColorDBL color, type MaterialType) : color(color), materialType(MaterialType) {}

Material::Material() : color(ColorDBL(1.0, 0.0, 0.0)), materialType(DIFFUSE) {}

ColorDBL Material::getColor() const { return color; }

Material::type Material::getMaterialType() const { return materialType; }
