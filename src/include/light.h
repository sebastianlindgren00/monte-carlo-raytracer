#include "include/glm/glm.hpp"
#include <algorithm>
#include "shape.h"
#include <random>
#include <corecrt_math_defines.h>

class Light {
public:
    Light(const glm::dvec3& topLeft, const glm::dvec3& topRight, const glm::dvec3& bottomLeft, const glm::dvec3& bottomRight);

    Shape* getLightShape();

    double random_double() const;

    ColorDBL computeIrradiance(const glm::dvec3& hitPoint, Shape* hitShape);
    
    std::vector<glm::dvec3> getVertices() const;
    
    
private:
    glm::dvec3 topLeft;
    glm::dvec3 topRight;
    glm::dvec3 bottomLeft;
    glm::dvec3 bottomRight;
    glm::dvec3 normal;
    const double WATT = 50.0; 
    Plane* plane;
};
