#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp> // For glm::normalize
#include <memory> // For std::shared_ptr
#include "colorDBL.h"

class Ray {
public:
    
    Ray(const glm::dvec3& origin, const glm::dvec3& direction, int depth = 0) : origin(origin), direction(direction), nextRay(nullptr), previousRay(nullptr) {}

    glm::dvec3 pointAtSurface(double point) const {
        return origin + point * direction;
    }

    glm::dvec3 origin; 
    glm::dvec3 direction;
    std::shared_ptr<Ray> previousRay;
    std::shared_ptr<Ray> nextRay;
    ColorDBL color;
    int depth;
};
