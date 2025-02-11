#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"
#include <algorithm>
#include "shape.h"
#include <vector>
#include <random>
#include <corecrt_math_defines.h>

class Light {
public:
    Light(const glm::dvec3& topLeft, const glm::dvec3& topRight, 
          const glm::dvec3& bottomLeft, const glm::dvec3& bottomRight);

    // Returns the light's geometry (for sampling, if needed)
    Shape* getLightShape();

    static double random_double();
    
    std::vector<glm::dvec3> getVertices() const;

    glm::dvec3 randomPointOnLight();
    
    glm::dvec3 topLeft;
    glm::dvec3 topRight;
    glm::dvec3 bottomLeft;
    glm::dvec3 bottomRight;
    glm::dvec3 normal;
    const double WATT = 25.0; 
    Plane* plane;
};

#endif // LIGHT_H
