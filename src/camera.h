#include <glm/vec3.hpp>
#include "ray.h"

class Camera {
public:
    Camera(int width, int height)
        : startPosition(glm::dvec3(-1.0f, 0.0f, 0.0f)), 
          direction(glm::normalize(glm::dvec3(1.0f, 0.0f, 0.0f))),
          c1(glm::dvec3(0.0f, -1.0f, -1.0f)),
          c2(glm::dvec3(0.0f, 1.0f, -1.0f)),
          c3(glm::dvec3(0.0f, 1.0f, 1.0f)),
          c4(glm::dvec3(0.0f, -1.0f, 1.0f)),
          width(width), height(height) {
        this->pixelSize = 2.0f / height;
    }

    Ray getRay(int i, int j) {
        float x = i * pixelSize - (1.0 - pixelSize); // see lecture 4
        float y = (height - j) * pixelSize - (1.0 - pixelSize); // had to go to -j to flip it 

        glm::dvec3 pixelPosition = glm::dvec3(0.0, x, y);
        glm::dvec3 rayDirection = pixelPosition - startPosition;
        return Ray(startPosition, rayDirection);
    }

    glm::dvec3 startPosition;
    glm::dvec3 direction;
    int width;
    int height;
    float pixelSize;
    glm::dvec3 c1, c2, c3, c4;
};
