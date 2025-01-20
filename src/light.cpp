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

double Light::random_double() const {
    return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
}

ColorDBL Light::computeIrradiance(const glm::dvec3& hitPoint, const Shape* hitShape) const {
    
    ColorDBL irradiance = ColorDBL(0.0, 0.0, 0.0);

    double u = random_double();
    double v = random_double();
    
    glm::dvec3 pointOnLight = topLeft + u * (topRight - topLeft) + v * (bottomLeft - topLeft);

    double distance = glm::distance(hitPoint, pointOnLight);
    double area = glm::length(glm::cross(topRight - topLeft, bottomLeft - topLeft));
    double cos_omega_x = glm::clamp(glm::dot(hitShape->getNormal(hitPoint), glm::normalize(pointOnLight - hitPoint)), 0.0, (double)INFINITY);
    double cos_omega_y = -1.0 * glm::dot(plane->getNormal(hitPoint), glm::normalize(pointOnLight - hitPoint));

    if(cos_omega_y < 0.0) { cos_omega_y = 0.0; }

    double G = cos_omega_x * cos_omega_y / (distance * distance);
    double E = area * G * WATT / M_PI;
    ColorDBL color = hitShape->getMaterial().color;
    //std::cout << "Color: " << color << std::endl;

    return irradiance += color * E;
}

std::vector<glm::dvec3> Light::getVertices() const {
    return {topLeft, topRight, bottomLeft, bottomRight};
}