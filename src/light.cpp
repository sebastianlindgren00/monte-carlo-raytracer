#include "include/light.h"

Light::Light(const glm::dvec3& topLeft, const glm::dvec3& topRight, 
        const glm::dvec3& bottomLeft, const glm::dvec3& bottomRight)
    : topLeft(topLeft), topRight(topRight), 
        bottomLeft(bottomLeft), bottomRight(bottomRight) {
        plane = new Plane(topLeft, topRight, bottomLeft, bottomRight, Material(ColorDBL::white(), Material::type::LIGHT));
        normal = plane->getNormal();
}

Shape* Light::getLightShape() {
    return plane;
}

double Light::random_double() {
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

std::vector<glm::dvec3> Light::getVertices() const {
    return {topLeft, topRight, bottomLeft, bottomRight};
}