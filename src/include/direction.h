#include <glm/glm.hpp>
#include <cmath>
#include <algorithm>
#include <corecrt_math_defines.h>

class Direction {
public:

    Direction(double azimuth, double inclination);
    void setDirection(double azimuth, double inclination);
    double getAzimuth() const;
    double getInclination() const;
    glm::dvec3 getDirection() const;
    glm::dvec3 HemisphericalToCartesianLocalSystem(Direction* direction); // lecture 2 DONE
    glm::dvec3 CartesianLocalSystemToCartesianWorldSystem(glm::dvec3 localSystem, const glm::dvec3& surfaceNormal); // lecture 2 DONE
    Direction CartesianWorldSystemToHemispherical(glm::dvec3 worldSystem); // lecture 2
    static Direction RandomDirectionWithBRDF(); // using Bidirectional Reflectance Distribution Function (BRDF) // lecture 3

private:
    double azimuth = 0.0; // lecture 1
    double inclination = 0.0; // lecture 1
    glm::dvec3 directionVector; 
};