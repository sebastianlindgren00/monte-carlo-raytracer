#include "include/ray.h"
#include "include/scene.h"
#include "include/shape.h"

Ray::Ray(const glm::dvec3& origin, const glm::dvec3& direction) : origin(origin), direction(direction), nextRay(nullptr), previousRay(nullptr) {}

glm::dvec3 Ray::pointAtSurface(double point) const {
    return origin + point * direction;
}

void Ray::traceRay(Scene* scene, int depth) {

    if (depth > MAX_DEPTH  || (double)rand() > 0.8) { // added a russian roulette with 20% chance of ray termination
        this->nextRay = nullptr;
        return;
    }

    ColorDBL color(0, 0, 0);
    double t_min = std::numeric_limits<double>::max();
    Shape* hitShape = nullptr;

    // intersection
    for (auto it = scene->shapes.begin(); it != scene->shapes.end(); ++it) {
        Shape* shape = *it;
        double t = shape->intersect(this);
        if (t > 0.0 && t < t_min) {
            t_min = t;
            hitShape = shape;
        }
    }

    if (hitShape) {
        glm::dvec3 hitPoint = pointAtSurface(t_min);
        glm::dvec3 normal = hitShape->getNormal();

        switch(hitShape->getMaterial().getMaterialType()){
            case Material::type::LIGHT: {
                // ray dies instantly
                return;
            }

            case Material::type::DIFFUSE: {
                Direction direction = Direction::RandomDirectionWithBRDF();
                glm::dvec3 localSystem = direction.HemisphericalToCartesianLocalSystem(&direction);
                glm::dvec3 worldSystem = direction.CartesianLocalSystemToCartesianWorldSystem(localSystem, normal);

                this->nextRay = new Ray(hitPoint, worldSystem);
                this->nextRay->previousRay = this;
                this->nextRay->traceRay(scene, depth + 1);
                break;
            }

            case Material::type::MIRROR: {
                this->nextRay = new Ray(hitPoint, glm::normalize(glm::reflect(this->direction, normal)));
                this->nextRay->previousRay = this;
                this->nextRay = this;
                this->nextRay->traceRay(scene, depth + 1);
                break;
            }
        }   
    }
}

ColorDBL Ray::computeIrradiance(const glm::dvec3& hitPoint, const Shape* hitshape, Light* light) const {
    
    ColorDBL irradiance = ColorDBL(0.0, 0.0, 0.0);

    double u = Light::random_double();
    double v = Light::random_double();
    
    glm::dvec3 pointOnLight = light->topLeft + u * (light->topRight - light->topLeft) + v * (light->bottomLeft - light->topLeft);

    double distance = glm::distance(hitPoint, pointOnLight);
    double area = glm::length(glm::cross(light->topRight - light->topLeft, light->bottomLeft - light->topLeft));
    double cos_omega_x = glm::clamp(glm::dot(hitshape->getNormal(), glm::normalize(pointOnLight - hitPoint)), 0.0, (double)INFINITY);
    double cos_omega_y = -1.0 * glm::dot(light->plane->getNormal(), glm::normalize(pointOnLight - hitPoint));

    if(cos_omega_y < 0.0) { cos_omega_y = 0.0; }

    double G = cos_omega_x * cos_omega_y / (distance * distance);
    double E = area * G * light->WATT / M_PI;
    ColorDBL color = hitshape->getMaterial().color;

    return irradiance += color * E;
}