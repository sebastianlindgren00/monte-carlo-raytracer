#include "light.h"
#include "shape.h"

Light::Light(const glm::dvec3& topLeft, const glm::dvec3& topRight, 
             const glm::dvec3& bottomLeft, const glm::dvec3& bottomRight)
    : topLeft(topLeft), topRight(topRight), bottomLeft(bottomLeft), bottomRight(bottomRight)
{
    plane = new Plane(topLeft, topRight, bottomLeft, bottomRight, Material(ColorDBL::white(), Material::type::LIGHT));
    normal = plane->getNormal();
}

Shape* Light::getLightShape() {
    return plane;
}

double Light::random_double() {
    return (double)rand() / (double)RAND_MAX;
}

std::vector<glm::dvec3> Light::getVertices() const {
    return {topLeft, topRight, bottomLeft, bottomRight};
}

glm::dvec3 Light::randomPointOnLight() {
    double u = random_double();
    double v = random_double();
    return topLeft + u * (topRight - topLeft) + v * (bottomLeft - topLeft);
}
