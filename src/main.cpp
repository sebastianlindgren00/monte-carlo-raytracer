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
        shapes.push_back(light.getLightShape());
    }

    ~Scene() {
        for (Shape* shape : shapes) {
            delete shape;
        }
    }
    // byt till parallell for (kör multi core, 800% increase :O)
    void render() {
        for (int j = 0; j < camera.height; j++) {
            for (int i = 0; i < camera.width; i++) {
                Ray ray = camera.getRay(i, j);
                ColorDBL color = trace(ray); 
                pixels.push_back(color); 
            }
        }
    }

    ColorDBL trace(const Ray& ray, int depth = 0) {
        ColorDBL color = ColorDBL(0, 0, 0); 
        double t_min = std::numeric_limits<double>::max();
        Shape* hitShape = nullptr;

        // Find the nearest intersection
        for (Shape* shape : shapes) {
            double t = shape->intersect(ray);
            //std::cout << shape->getMaterial().color << std::endl;
            if (t > 0.0 && t < t_min) {
                t_min = t;
                hitShape = shape;
            }
        }

        if (hitShape) {
            // Calculate hit point and normal
            glm::dvec3 hitPoint = ray.origin + static_cast<double>(t_min) * ray.direction;

            // Diffuse lighting calculation
            ColorDBL diffuseColor = light.computeDiffuse(hitPoint, hitShape);
            color += diffuseColor;
        }
        //std::cout << color << std::endl;
        return color;
    }


    void saveImage(const std::string& filename) {
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

private:
    Camera camera;
    std::vector<Shape*> shapes;
    std::vector<ColorDBL> pixels;
    Light light;
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