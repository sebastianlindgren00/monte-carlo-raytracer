#ifndef RAY_H
#define RAY_H

#include "colorDBL.h"
#include "material.h"
#include <glm/glm.hpp>
#include "direction.h"
#include <memory>

class Scene;
class Shape;
class Light;

class Ray {
public:
    Ray(const glm::dvec3& origin, const glm::dvec3& direction, int depth = 0);

    glm::dvec3 pointAtSurface(double point) const;

    void traceRay(Scene* scene);
    void releaseRayChain();

    ColorDBL computeDiffuseRadiance(Scene* scene, const glm::dvec3& hitPoint, Shape* hitshape, Light* light) const;
    double isShadowed(Scene* scene, const glm::dvec3& hitPoint, const glm::dvec3& pointOnLight, Light* light) const;
    void PixelRayColor(Scene* scene);

    glm::dvec3 origin; 
    glm::dvec3 direction;
    Ray* previousRay;
    Ray* nextRay;
    ColorDBL color;
    int depth;
    const int MAX_DEPTH = 2;
    const int MAX_SHADOW_RAYS = 10;
};

#endif // RAY_H
