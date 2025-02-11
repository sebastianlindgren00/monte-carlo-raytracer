#include "ray.h"
#include "scene.h"
#include "shape.h"
#include "light.h"
#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/constants.hpp>

Ray::Ray(const glm::dvec3& origin, const glm::dvec3& direction, int depth)
    : origin(origin), direction(direction), nextRay(nullptr), previousRay(nullptr), depth(depth) {}

glm::dvec3 Ray::pointAtSurface(double point) const {
    return origin + point * direction;
}

void Ray::releaseRayChain() {
    if (nextRay != nullptr) {
        nextRay->releaseRayChain();
        delete nextRay;
        nextRay = nullptr;
    }
}

void Ray::traceRay(Scene* scene) {
    // russian roulette and depth check
    if (depth > MAX_DEPTH || ((double)rand() / RAND_MAX) > 0.75) {
        this->nextRay = nullptr;
        return;
    }

    double t_min = 0.0;
    Shape* hitShape = nullptr;

    if (scene->findNearestIntersection(this, hitShape, t_min)) {
        glm::dvec3 hitPoint = pointAtSurface(t_min) - 0.001 * direction;
        glm::dvec3 normal = hitShape->getNormal(hitPoint);

        switch(hitShape->getMaterial().getMaterialType()){
            case Material::type::LIGHT: {
                // When a ray hits the light, we stop tracing further bounces.
                return;
            }
            case Material::type::DIFFUSE: {
                Direction newDir = Direction::RandomDirection();
                glm::dvec3 localSystem = newDir.HemisphericalToCartesianLocalSystem(&newDir);
                glm::dvec3 worldSystem = newDir.CartesianLocalSystemToCartesianWorldSystem(localSystem, normal);

                this->nextRay = new Ray(hitPoint, worldSystem, this->depth + 1);
                this->nextRay->previousRay = this;
                this->nextRay->traceRay(scene);
                break;
            }
            case Material::type::MIRROR: {
                glm::dvec3 reflectDir = glm::normalize(glm::reflect(this->direction, normal));
                this->nextRay = new Ray(hitPoint, reflectDir, this->depth + 1);
                this->nextRay->previousRay = this;
                this->nextRay->traceRay(scene);
                break;
            }
            default:
                break;
        }
    }
}

void Ray::PixelRayColor(Scene* scene) {
    Ray* lastRay = this;
    while (lastRay->nextRay != nullptr) {
        lastRay = lastRay->nextRay;
    }

    // start at the end
    {
        double t_min = 0.0;
        Shape* hitShape = nullptr;
        if (scene->findNearestIntersection(lastRay, hitShape, t_min)) {
            glm::dvec3 hitPoint = lastRay->pointAtSurface(t_min) - 0.001 * lastRay->direction;
            switch (hitShape->getMaterial().getMaterialType()) {
                case Material::type::LIGHT:
                    lastRay->color = hitShape->getMaterial().getColor();
                    break;
                case Material::type::DIFFUSE:
                    lastRay->color = lastRay->computeDiffuseRadiance(scene, hitPoint, hitShape, &scene->light);
                    break;
                case Material::type::MIRROR:
                    lastRay->color = ColorDBL(0, 0, 0);
                    break;
                default:
                    lastRay->color = ColorDBL(0, 0, 0);
                    break;
            }
        } else {
            // nothing at end
            lastRay->color = ColorDBL(0, 0, 0);
        }
    }

    // start at the end and go back
    Ray* currentRay = lastRay;
    while (currentRay->previousRay != nullptr) {
        Ray* prevRay = currentRay->previousRay;
        double t_min = 0.0;
        Shape* hitShape = nullptr;
        if (scene->findNearestIntersection(prevRay, hitShape, t_min)) {
            glm::dvec3 hitPoint = prevRay->pointAtSurface(t_min) - 0.001 * prevRay->direction;
            switch (hitShape->getMaterial().getMaterialType()) {
                case Material::type::LIGHT:
                    prevRay->color = hitShape->getMaterial().getColor();
                    break;
                case Material::type::DIFFUSE: {
                    ColorDBL diffuseRadiance = prevRay->computeDiffuseRadiance(scene, hitPoint, hitShape, &scene->light);
                    prevRay->color = diffuseRadiance + (hitShape->getMaterial().getColor() * currentRay->color);
                    break;
                }
                case Material::type::MIRROR:
                    // return last
                    prevRay->color = currentRay->color;
                    break;
                default:
                    prevRay->color = currentRay->color;
                    break;
            }
        } else {
            prevRay->color = currentRay->color; // i think this was the problem?
        }
        currentRay = prevRay;
    }

    this->color = currentRay->color;
    this->releaseRayChain(); // free up space
}

ColorDBL Ray::computeDiffuseRadiance(Scene* scene, const glm::dvec3& hitPoint, Shape* hitShape, Light* light) const {
    glm::dvec3 normal = hitShape->getNormal(hitPoint);
    const double EPSILON = 0.001;

    glm::dvec3 offsetHitPoint = hitPoint + normal * EPSILON;

    double u = Light::random_double();
    double v = Light::random_double();
    glm::dvec3 pointOnLight = light->topLeft 
    + u * (light->topRight - light->topLeft)
    + v * (light->bottomLeft - light->topLeft);

    glm::dvec3 toLight = pointOnLight - offsetHitPoint;
    double distanceSq = glm::dot(toLight, toLight);
    glm::dvec3 L = glm::normalize(toLight);

    double cosThetaSurface = glm::dot(normal, L);
 
    cosThetaSurface = glm::clamp(cosThetaSurface, 0.0, 1.0);

    double cosThetaLight = glm::max(-glm::dot(light->plane->getNormal(), L), 0.0);

    double G = (cosThetaSurface * cosThetaLight) / distanceSq;

    glm::dvec3 edge1 = light->topRight - light->topLeft;
    glm::dvec3 edge2 = light->bottomLeft - light->topLeft;
    double area = glm::length(glm::cross(edge1, edge2));

    double E_inc = area * G * light->WATT;

    double shadowFactor = isShadowed(scene, offsetHitPoint, pointOnLight, light);

    ColorDBL albedo = hitShape->getMaterial().getColor();

    ColorDBL diffuseRadiance = (albedo / M_PI) * (E_inc * shadowFactor);

    return diffuseRadiance;
}

double Ray::isShadowed(Scene* scene, const glm::dvec3& hitPoint, const glm::dvec3& pointOnLight, Light* light) const {
    Ray shadowRay(hitPoint, pointOnLight - hitPoint);
    
    Shape* hitShape = nullptr;
    double t_min = 0.0;
    
    if(scene->findNearestIntersection(&shadowRay, hitShape, t_min)) {
        if (hitShape != nullptr && hitShape->getMaterial().getMaterialType() != Material::type::LIGHT) {
            return 0.0; 
        }        
    }

    return 1.0;
}