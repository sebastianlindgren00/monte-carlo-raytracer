#include "include/ray.h"    
#include <glm/vec3.hpp>

class Camera {
public:
    Camera(int width, int height);
    Ray* getRay(int i, int j);

    glm::dvec3 startPosition;
    glm::dvec3 direction;
    int width;
    int height;
    double pixelSize;
    glm::dvec3 c1, c2, c3, c4;
};
