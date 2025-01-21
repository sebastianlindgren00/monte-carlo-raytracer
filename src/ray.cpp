#include "include/ray.h"
#include "include/scene.h"
#include "include/shape.h"

Ray::Ray(const glm::dvec3& origin, const glm::dvec3& direction, int depth) : origin(origin), direction(direction), nextRay(nullptr), previousRay(nullptr) {}

glm::dvec3 Ray::pointAtSurface(double point) const {
    return origin + point * direction;
}

void Ray::traceRay(Scene* scene, int depth) {

    if (depth > MAX_DEPTH) {
        return;
    }

    ColorDBL color(0, 0, 0);
    double t_min = std::numeric_limits<double>::max();
    Shape* hitShape = nullptr;

    // Find the nearest intersection
    for (auto it = scene->shapes.begin(); it != scene->shapes.end(); ++it) {
        Shape* shape = *it;
        double t = shape->intersect(this);
        if (t > 0.0 && t < t_min) {
            t_min = t;
            hitShape = shape;
        }
    }

    if (hitShape) {
        std::cout << hitShape->getMaterial().getMaterialType() << std::endl;
        glm::dvec3 hitPoint = pointAtSurface(t_min);
        glm::dvec3 normal = hitShape->getNormal(hitPoint);

        switch(hitShape->getMaterial().getMaterialType()){
            case Material::type::LIGHT: {
                // ray dör instantly
                return;
            }

            case Material::type::DIFFUSE: {
                Ray* newRay = new Ray(hitPoint, this->direction, this->depth + 1);
                newRay->previousRay = this;
                this->nextRay = newRay;
                newRay->traceRay(scene, depth + 1);
            }

            case Material::type::MIRROR: {
                Ray* newRay = new Ray(hitPoint, glm::normalize(glm::reflect(this->direction, normal)), this->depth + 1);
                newRay->previousRay = this;
                this->nextRay = this;
                newRay->traceRay(scene, depth + 1);
            }
        }   
    }
}