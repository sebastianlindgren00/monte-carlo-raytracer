#include "include/direction.h"

Direction::Direction(double azimuth, double inclination) : azimuth(azimuth), inclination(inclination) {}

double Direction::getAzimuth() const {
    return azimuth;
}

double Direction::getInclination() const {
    return inclination;
}

glm::dvec3 Direction::getDirection() const {
    return directionVector;
}

glm::dvec3 Direction::HemisphericalToCartesianLocalSystem(Direction* direction) {
    double azimuth = direction->getAzimuth();
    double inclination = direction->getInclination();
    double x = std::cos(inclination) * std::cos(azimuth);
    double y = std::cos(inclination) * std::sin(azimuth);
    double z = std::sin(inclination);
    return glm::dvec3(x, y, z);
} 

glm::dvec3 Direction::CartesianLocalSystemToCartesianWorldSystem(glm::dvec3 localSystem, const glm::dvec3& surfaceNormal) {
    glm::dvec3 worldSystem = glm::dvec3(0.0, 0.0, 0.0);
    double x_0 = localSystem.x;
    double y_0 = localSystem.y;
    double z_0 = localSystem.z;

    // Local system
    glm::dvec3 z_L = surfaceNormal;
    glm::dvec3 x_L = glm::cross(glm::dvec3(0.0, 0.0, 1.0), z_L);   
    glm::dvec3 y_L = glm::cross(z_L, x_L);

    // World system
    double x_W = x_0 * x_L.x + y_0 * y_L.x + z_0 * z_L.x;
    double y_W = x_0 * x_L.y + y_0 * y_L.y + z_0 * z_L.y;
    double z_W = x_0 * x_L.z + y_0 * y_L.z + z_0 * z_L.z;

    worldSystem = glm::dvec3(x_W, y_W, z_W);

    return worldSystem;
}

Direction Direction::CartesianWorldSystemToHemispherical(glm::dvec3 worldSystem) {
    double x = worldSystem.x;
    double y = worldSystem.y;
    double z = worldSystem.z;
    double azimuth = std::atan2(y, x);
    double inclination = std::asin(z);
    return Direction(azimuth, inclination);
}

Direction Direction::RandomDirection() {
    double r1 = rand() / (double)RAND_MAX;
    double r2 = rand() / (double)RAND_MAX;
    double azimuth = 2 * M_PI * r1;
    double inclination = std::acos(std::sqrt(1.0 - r2)); 
    return Direction(azimuth, inclination);
}