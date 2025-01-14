#include "include/glm/glm.hpp"
#include <algorithm>
#include "shape.h"
#include <random>

class Light {
public:
    Light(const glm::dvec3& topLeft, const glm::dvec3& topRight, 
          const glm::dvec3& bottomLeft, const glm::dvec3& bottomRight)
        : topLeft(topLeft), topRight(topRight), 
          bottomLeft(bottomLeft), bottomRight(bottomRight) {
            normal = plane.getNormal(topLeft);
            color = white;
          }

    double random_double() const {
        return static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
    }
    
    glm::dvec3 computeDiffuse(const glm::dvec3& hitPoint, const glm::dvec3& surfaceNormal) const {
        
        double u = random_double();
        double v = random_double();
        
        glm::dvec3 pointOnLight = (1 - u) * (1 - v) * topLeft + 
                                u * (1 - v) * topRight + 
                                (1 - u) * v * bottomLeft + 
                                u * v * bottomRight;
        
        // Compute the direction from the hit point to the sampled point on the light
        glm::dvec3 lightDir = glm::normalize(pointOnLight - hitPoint);
        
        // Lambertian reflectance
        double diffuse = glm::max(glm::dot(surfaceNormal, lightDir), 0.0);
        
        return color * diffuse;
    }
    
private:
    glm::dvec3 topLeft;
    glm::dvec3 topRight;
    glm::dvec3 bottomLeft;
    glm::dvec3 bottomRight;
    glm::dvec3 color;
    glm::dvec3 normal;
    glm::dvec3 white = glm::dvec3(1.0, 1.0, 1.0);
    DiffusePlane plane{topLeft, topRight, bottomLeft, bottomRight, white};
};
