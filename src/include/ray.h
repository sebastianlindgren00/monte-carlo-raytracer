#ifndef RAY_H
#define RAY_H

#include "include/colorDBL.h"
#include "include/material.h"
#include <glm/glm.hpp>
#include "include/direction.h"
#include <memory> // For std::shared_ptr

class Scene;
class Shape;
class Light;

class Ray {
public:
    
    Ray(const glm::dvec3& origin, const glm::dvec3& direction);

    glm::dvec3 pointAtSurface(double point) const;

    void traceRay(Scene* scene, int depth);

    ColorDBL computeIrradiance(const glm::dvec3& hitPoint, const Shape* hitshape, Light* light) const;

    glm::dvec3 origin; 
    glm::dvec3 direction;
    Ray* previousRay;
    Ray* nextRay;
    ColorDBL color;
    int depth;
    const int MAX_DEPTH = 2;
};
#endif // RAY_H
