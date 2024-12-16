#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp> // For glm::normalize

class Ray {
public:
    // Constructor to initialize origin and direction
    Ray(const glm::dvec3& origin, const glm::dvec3& direction)
        : origin(origin), direction(glm::normalize(direction)) {}

    // Accessor methods for safety (optional)
    const glm::dvec3& getOrigin() const { return origin; }
    const glm::dvec3& getDirection() const { return direction; }

    // Computes a point along the ray at parameter 't'
    glm::dvec3 at(float t) const {
        return origin + static_cast<double>(t) * direction;
    }

    glm::dvec3 origin;    // Ray origin point
    glm::dvec3 direction; // Normalized ray direction
};
