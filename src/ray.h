#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp> // For glm::normalize

class Ray {
public:
    // Constructor to initialize origin and direction
    Ray(const glm::vec3& origin, const glm::vec3& direction)
        : origin(origin), direction(glm::normalize(direction)) {}

    // Accessor methods for safety (optional)
    const glm::vec3& getOrigin() const { return origin; }
    const glm::vec3& getDirection() const { return direction; }

    // Computes a point along the ray at parameter 't'
    glm::vec3 at(float t) const {
        return origin + t * direction;
    }

    glm::vec3 origin;    // Ray origin point
    glm::vec3 direction; // Normalized ray direction
};
