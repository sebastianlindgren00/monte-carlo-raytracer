#include "direction.h"

Direction::Direction(double azimuth, double inclination) : azimuth(azimuth), inclination(inclination) {}

void Direction::setDirection(double azimuth, double inclination) {
    this->azimuth = azimuth;
    this->inclination = inclination;
}

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
    glm::dvec3 z_L = glm::normalize(surfaceNormal);
    // if surfaceNormal is nearly vertical, use alternative up vector instead
    glm::dvec3 up = (std::abs(z_L.y) < 0.999) ? glm::dvec3(0.0, 1.0, 0.0) : glm::dvec3(1.0, 0.0, 0.0);
    glm::dvec3 x_L = glm::normalize(glm::cross(up, z_L));
    glm::dvec3 y_L = glm::cross(z_L, x_L);

    double x_W = localSystem.x * x_L.x + localSystem.y * y_L.x + localSystem.z * z_L.x;
    double y_W = localSystem.x * x_L.y + localSystem.y * y_L.y + localSystem.z * z_L.y;
    double z_W = localSystem.x * x_L.z + localSystem.y * y_L.z + localSystem.z * z_L.z;

    return glm::normalize(glm::dvec3(x_W, y_W, z_W));
}

Direction Direction::CartesianWorldSystemToHemispherical(glm::dvec3 worldSystem) {
    double x = worldSystem.x;
    double y = worldSystem.y;
    double z = worldSystem.z;
    double azimuth = glm::sign(y) * acos(x / (sqrt(x*x + y*y)));
    double inclination = acos(z / (sqrt(x*x + y*y + z*z)));
    return Direction(azimuth, inclination);
}

Direction Direction::RandomDirection() {
    double r1 = (double)rand() / (double)RAND_MAX;
    double r2 = (double)rand() / (double)RAND_MAX;
    double azimuth = 2.0 * M_PI * r1;
    double inclination = acos(std::sqrt(1.0 - r2));
    return Direction(azimuth, inclination);
}
