#ifndef DIRECTION_H
#define DIRECTION_H

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
    glm::dvec3 HemisphericalToCartesianLocalSystem(Direction* direction);
    glm::dvec3 CartesianLocalSystemToCartesianWorldSystem(glm::dvec3 localSystem, const glm::dvec3& surfaceNormal);
    Direction CartesianWorldSystemToHemispherical(glm::dvec3 worldSystem);
    static Direction RandomDirection();

private:
    double azimuth = 0.0;
    double inclination = 0.0;
    glm::dvec3 directionVector;
};

#endif // DIRECTION_H
