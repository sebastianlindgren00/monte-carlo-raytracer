#include "camera.h"
#include "shape.h"
#include "light.h"
#include "colorDBL.h"
#include <vector>
#include <iostream>
#include <fstream>

class Scene {
public:
    Scene() : camera(800, 800), light(glm::vec3(0, 10, 0), glm::vec3(1.0f, 1.0f, 1.0f)) { // example light position and color
        shapes = ShapeFactory::createShapes();
    }

    ~Scene() {
        for (Shape* shape : shapes) {
            delete shape;
        }
    }

    void render() {
        for (int i = 0; i < camera.height; i++) {
            for (int j = 0; j < camera.width; j++) {
                Ray ray = camera.getRay(i, j);
                ColorDBL color = trace(ray); // Return ColorDBL
                pixels.push_back(color); // Cache result for saving
            }
        }
    }

    glm::vec3 computeReflection(const glm::vec3& rayDir, const glm::vec3& normal) {
        return glm::normalize(rayDir - 2.0f * glm::dot(rayDir, normal) * normal);
    }

    ColorDBL trace(const Ray& ray, int depth = 0) {
        const int maxDepth = 2; // Set a maximum bounce limit
        ColorDBL color = ColorDBL(0, 0, 0); // Background color
        float t_min = 1e10f;                  // Large initial value for nearest intersection
        Shape* hitShape = nullptr;

        // Find the nearest intersection
        for (Shape* shape : shapes) {
            float t = shape->intersect(ray);
            if (t > 0 && t < t_min) {
                t_min = t;
                hitShape = shape;
            }
        }

        if (hitShape) {
            // Calculate hit point and normal
            glm::vec3 hitPoint = ray.origin + t_min * ray.direction;
            glm::vec3 normal = hitShape->getNormal(hitPoint);

            // Diffuse lighting calculation
            glm::vec3 diffuseColor = light.computeDiffuse(hitPoint, normal);
            color += ColorDBL(hitShape->color * diffuseColor);

            // Check for reflections if within max depth
            if (depth < maxDepth) {
                
                glm::vec3 reflectionDir = computeReflection(ray.direction, normal);
                Ray reflectionRay(hitPoint + normal, reflectionDir); 

                // Recursive trace call for reflected ray
                ColorDBL reflectionColor = trace(reflectionRay, depth + 1);
                float reflectivity = 0.5f; // Adjust reflectivity as a material property (0.0 to 1.0)
                glm::vec3 mixedColor = glm::mix(glm::vec3(color.r, color.g, color.b), glm::vec3(reflectionColor.r, reflectionColor.g, reflectionColor.b), reflectivity);
                color = ColorDBL(mixedColor.r, mixedColor.g, mixedColor.b);
            }
        }
        return color;
    }

    void saveImage(const std::string& filename) {
        std::ofstream file(filename, std::ios::out | std::ios::binary);

        file << "P6\n" << camera.width << " " << camera.height << "\n255\n";

        for (const auto& rgb : pixels) {
            glm::vec3 color = rgb.toRGB();
            file.put(static_cast<unsigned char>(glm::clamp(color.r, 0.0f, 255.0f)));
            file.put(static_cast<unsigned char>(glm::clamp(color.g, 0.0f, 255.0f)));
            file.put(static_cast<unsigned char>(glm::clamp(color.b, 0.0f, 255.0f)));
        }
        file.close();
    }

private:
    Camera camera;
    std::vector<Shape*> shapes;
    std::vector<ColorDBL> pixels;
    Light light;
};

int main () {
    Scene scene;
    scene.render();
    scene.saveImage("/monte-carlo-raytracer/src/image/result.ppm");
    std::cout << "Image saved" << std::endl;
    return 0;
}
