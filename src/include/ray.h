#ifndef RAY_H
#define RAY_H

#include "include/colorDBL.h"
#include "include/material.h"
#include <glm/vec3.hpp>
#include <glm/geometric.hpp> // For glm::normalize
#include <memory> // For std::shared_ptr

class Scene;
class Shape;

class Ray {
public:
    
    Ray(const glm::dvec3& origin, const glm::dvec3& direction, int depth = 0);

    glm::dvec3 pointAtSurface(double point) const;

    void traceRay(Scene* scene, int depth);

    glm::dvec3 origin; 
    glm::dvec3 direction;
    Ray* previousRay;
    Ray* nextRay;
    ColorDBL color;
    int depth;
    const int MAX_DEPTH = 5;
};
#endif // RAY_H
