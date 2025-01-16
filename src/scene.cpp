#include "include/scene.h"

Scene::Scene() : camera(800, 800), light(glm::dvec3(5.0, 1.5, 4.90), glm::dvec3(5.0, -1.5, 4.90), glm::dvec3(8.0, 1.5, 4.90), glm::dvec3(8.0, -1.5, 4.90)) { 
    shapes = ShapeFactory::createShapes();
    shapes.push_back(light.getLightShape());
}

Scene::~Scene() {
    for (Shape* shape : shapes) {
        delete shape;
    }
}

void Scene::render() {
    for (int j = 0; j < camera.height; j++) {
        for (int i = 0; i < camera.width; i++) {
            Ray* ray = camera.getRay(i, j);
            //ColorDBL color = traceRay(ray); // retyurns 1st or last ray
            ray->traceRay(this, 0);
            ColorDBL color = PixelRayColor(ray);
            pixels.push_back(color); 
        }
    }
}

ColorDBL Scene::PixelRayColor(Ray* ray, int maxDepth) {
    ColorDBL color(0, 0, 0);
    double t_min = std::numeric_limits<double>::max();
    Shape* hitShape = nullptr;

    // Find the nearest intersection
    for (Shape* shape : shapes) {
        double t = shape->intersect(ray);
        if (t > 0.0 && t < t_min) {
            t_min = t;
            hitShape = shape;
        }
    }

    if (hitShape) {
        glm::dvec3 hitPoint = ray->pointAtSurface(t_min);
        glm::dvec3 normal = hitShape->getNormal();

        switch(hitShape->getMaterial().getMaterialType()){
            case Material::type::LIGHT: {
                return hitShape->getMaterial().getColor();
            }

            case Material::type::DIFFUSE: {
                
                ColorDBL diffuseColor = light.computeIrradiance(hitPoint, hitShape);
                color += diffuseColor;
                return color;
            }

            case Material::type::MIRROR: {
                
                // glm::dvec3 reflectionDirection = glm::normalize(ray.direction - 2.0 * glm::dot(ray.direction, normal) * normal);
                // Ray reflectionRay(hitPoint + 0.001 * reflectionDirection, reflectionDirection, ray.depth + 1);
                // ColorDBL reflectionColor = trace(reflectionRay);
            }
        }   
    }
    return color;
}

void Scene::saveImage(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    file << "P6\n" << camera.width << " " << camera.height << "\n255\n";
    for (const auto& rgb : pixels) {
        glm::dvec3 color = rgb.toRGB();
        file.put(static_cast<unsigned char>(glm::clamp((color.r), 0.0, 255.0)));
        file.put(static_cast<unsigned char>(glm::clamp((color.g), 0.0, 255.0)));
        file.put(static_cast<unsigned char>(glm::clamp((color.b), 0.0, 255.0)));
    }
    file.close();
}
