#include "camera.h"
#include "ray.h"
#include <glm/gtc/constants.hpp>

Camera::Camera(int width, int height)
    : startPosition(glm::dvec3(-1.0, 0.0, 0.0)),
      direction(glm::normalize(glm::dvec3(1.0, 0.0, 0.0))),
      c1(glm::dvec3(0.0, -1.0, -1.0)),
      c2(glm::dvec3(0.0, 1.0, -1.0)),
      c3(glm::dvec3(0.0, 1.0, 1.0)),
      c4(glm::dvec3(0.0, -1.0, 1.0)),
      width(width), height(height)
{
    this->pixelSize = 2.0 / height;
}

Ray* Camera::getRay(int i, int j) {
    double x = i * pixelSize - (1.0 - pixelSize);
    double y = (height - j) * pixelSize - (1.0 - pixelSize);
    glm::dvec3 pixelPosition = glm::dvec3(0.0, x, y);
    glm::dvec3 rayDirection = pixelPosition - startPosition;
    return new Ray(startPosition, rayDirection);
}
