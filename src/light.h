#include "include/glm/glm.hpp"
#include <algorithm>

class Light {
public:
    Light(const glm::dvec3& position, const glm::dvec3& color) 
        : position(position), color(color) {}

    // Diffuse lighting calculation
    virtual glm::dvec3 computeDiffuse(const glm::dvec3& hitPoint, const glm::dvec3& normal) {
        glm::dvec3 lightDir = glm::normalize(position - hitPoint);
        float diffuse = glm::max(glm::dot(normal, lightDir), 0.0);
        return color * glm::dvec3(diffuse);
    }

    glm::dvec3 position;
    glm::dvec3 color;
};

// Area light, point light, spot light, etc. can be added here

class AreaLight : public Light {
public:
    AreaLight(const glm::dvec3& position, const glm::dvec3& color, const glm::dvec3& normal, float width, float height)
        : Light(position, color), normal(normal), width(width), height(height) {}

    // Diffuse lighting calculation
    glm::dvec3 computeDiffuse(const glm::dvec3& hitPoint, const glm::dvec3& normal) {
        glm::dvec3 lightDir = glm::normalize(position - hitPoint);
        float diffuse = glm::max(glm::dot(normal, lightDir), 0.0);  // Lambertian reflectance
        return color * static_cast<double>(diffuse);  // Simple diffuse lighting
    }


    glm::dvec3 normal;
    float width;
    float height;
};

