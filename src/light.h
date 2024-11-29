#include "include/glm/glm.hpp"

class Light {
public:
    Light(const glm::vec3& position, const glm::vec3& color) 
        : position(position), color(color) {}

    // Diffuse lighting calculation
    glm::vec3 computeDiffuse(const glm::vec3& hitPoint, const glm::vec3& normal) {
        glm::vec3 lightDir = glm::normalize(position - hitPoint);
        float diffuse = glm::max(glm::dot(normal, lightDir), 0.0f);
        return color * diffuse;
    }

    glm::vec3 position;
    glm::vec3 color;
};
