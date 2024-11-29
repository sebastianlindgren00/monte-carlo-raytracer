#include <glm/vec3.hpp>
#include "ray.h"

class Camera {
public:
    Camera(int width, int height)
        : startPosition(glm::vec3(-1.0f, 0.0f, 0.0f)), 
          direction(glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f))),
          c1(glm::vec3(0.0f, -1.0f, -1.0f)),
          c2(glm::vec3(0.0f, 1.0f, -1.0f)),
          c3(glm::vec3(0.0f, 1.0f, 1.0f)),
          c4(glm::vec3(0.0f, -1.0f, 1.0f)),
          width(width), height(height) {
        this->pixelSize = 2.0f / height;
    }

    Ray getRay(int i, int j) {
        float u = j * pixelSize - (1.0f - pixelSize);
        float v = i * pixelSize - (1.0f - pixelSize);

        glm::vec3 pixelPosition = glm::vec3(0.0f, u, v);
        glm::vec3 rayDirection = glm::normalize(pixelPosition - startPosition);
        return Ray(startPosition, rayDirection);
    }

    glm::vec3 startPosition;
    glm::vec3 direction;
    int width;
    int height;
    float pixelSize;
    glm::vec3 c1, c2, c3, c4;
};
