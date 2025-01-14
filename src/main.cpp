#include "camera.h"
#include "shape.h"
#include "light.h"
#include "colorDBL.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <random>

class Scene {
public:
    Scene() : camera(800, 800), light(glm::dvec3(5.0, 1.5, 4.90), glm::dvec3(5.0, -1.5, 4.90), glm::dvec3(8.0, 1.5, 4.90), glm::dvec3(8.0, -1.5, 4.90)) { // example light position and color
        shapes = ShapeFactory::createShapes();
    }

    ~Scene() {
        for (Shape* shape : shapes) {
            delete shape;
        }
    }

    void render() {
        for (int j = 0; j < camera.height; j++) {
            for (int i = 0; i < camera.width; i++) {
                Ray ray = camera.getRay(i, j);
                ColorDBL color = trace(ray); 
                pixels.push_back(color); 
            }
        }
    }

    glm::dvec3 computeReflection(const glm::dvec3& rayDir, const glm::dvec3& normal) {
        return glm::normalize(rayDir - 2.0f * glm::dot(rayDir, normal) * normal);
    }
    float random_float() {
        static std::uniform_real_distribution<float> distribution(0.0, 1.0);
        static std::mt19937 generator;
        return distribution(generator);
    }

    ColorDBL trace(const Ray& ray, int depth = 0) {
        ColorDBL color = ColorDBL(0, 0, 0); // black for background
        float t_min = std::numeric_limits<float>::max();
        Shape* hitShape = nullptr;

        // Find the nearest intersection
        for (Shape* shape : shapes) {
            float t = shape->intersect(ray);
            if (t > 0.0 && t < t_min) {
                t_min = t;
                hitShape = shape;
            }
        }

        if (hitShape) {
            // Calculate hit point and normal
            glm::dvec3 hitPoint = ray.origin + static_cast<double>(t_min) * ray.direction;
            glm::dvec3 normal = hitShape->getNormal(hitPoint);

            // Diffuse lighting calculation
            glm::dvec3 diffuseColor = light.computeDiffuse(hitPoint, normal);
            color += ColorDBL(hitShape->color * diffuseColor);
        }

        return color;
    }


    void saveImage(const std::string& filename) {
        std::ofstream file(filename, std::ios::out | std::ios::binary);

        file << "P6\n" << camera.width << " " << camera.height << "\n255\n";

        for (const auto& rgb : pixels) {
            glm::dvec3 color = rgb.toRGB();
            file.put(static_cast<unsigned char>(glm::clamp(static_cast<float>(color.r), 0.0f, 255.0f)));
            file.put(static_cast<unsigned char>(glm::clamp(static_cast<float>(color.g), 0.0f, 255.0f)));
            file.put(static_cast<unsigned char>(glm::clamp(static_cast<float>(color.b), 0.0f, 255.0f)));
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

    const std::string filePath = "/Users/sebastianlindgren/Documents/GitHub/monte-carlo-raytracer/src/image/result.ppm";
    scene.saveImage(filePath);
    std::cout << "Image saved to " << filePath << std::endl;

    system(("code " + filePath).c_str());

    return 0;
}