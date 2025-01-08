#include "camera.h"
#include "shape.h"
#include "light.h"
#include "colorDBL.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <corecrt_math_defines.h>

class Scene {
public:
    Scene() : camera(800, 800), areaLight(glm::dvec3(5.0f, 0.0f, 5.0f), glm::dvec3(1.0f, 1.0f, 1.0f)) { // example light position and color
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
                ColorDBL color = trace(ray); // Return ColorDBL
                pixels.push_back(color); // Cache result for saving
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

    glm::dvec3 RandomHemisphereDirection(const glm::dvec3& normal) {
        float phi = 2.0f * M_PI * random_float();
        float cos_theta = sqrt(1.0f - random_float());
        float sin_theta = sqrt(random_float());
        glm::dvec3 randomDir(sin_theta * cos(phi), sin_theta * sin(phi), cos_theta);

        // Adjust the random direction to be in the hemisphere around the normal
        return glm::normalize(randomDir + normal);
    }


    ColorDBL trace(const Ray& ray, int depth = 0) {
        ColorDBL color = ColorDBL(0, 0, 0); // black for background
        float t_min = 1e10f;
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
            glm::dvec3 hitPoint = ray.origin + static_cast<double>(t_min) * ray.direction;
            glm::dvec3 normal = hitShape->getNormal(hitPoint);

            // Diffuse lighting calculation
            glm::dvec3 diffuseColor = areaLight.computeDiffuse(hitPoint, normal);
            color += ColorDBL(hitShape->color * diffuseColor);

            if (hitShape->type == "DIFFUSE") {
                // Lambertian termination with a 50% chance of ray continuation
                float LambertianReflectionProb = 0.5f;  // Could be based on albedo
                if (random_float() > LambertianReflectionProb) {
                    return color;  // Terminate the ray if not scattered
                }

                // Scatter ray randomly in the hemisphere around the normal
                glm::dvec3 scatteredDirection = RandomHemisphereDirection(normal);
                Ray scatteredRay(hitPoint + normal * 0.001, scatteredDirection);  // Small offset to avoid self-intersection

                // Recursive trace call for scattered ray
                ColorDBL scatteredColor = trace(scatteredRay, depth + 1);
                return scatteredColor;  // Return the scattered color
            }
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
    AreaLight areaLight;
};

int main () {
    Scene scene;
    scene.render();

    const std::string filePath = "E:/Git/monte-carlo-raytracer/src/image/result.ppm";
    scene.saveImage(filePath);
    std::cout << "Image saved to " << filePath << std::endl;

    system(("code " + filePath).c_str());

    return 0;
}
