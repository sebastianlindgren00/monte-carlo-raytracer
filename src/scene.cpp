#include "scene.h"
#include "shape.h"
#include "ray.h"
#include "material.h"
#include "colorDBL.h"
#include "light.h"
#include <thread>
#include <ppl.h>
#include <atomic>
#include <mutex>
#include <chrono>

Scene::Scene() 
    : camera(800, 800),
      light(glm::dvec3(5.0, 1.5, 4.99),
      glm::dvec3(5.0, -1.5, 4.99),
      glm::dvec3(8.0, 1.5, 4.99),
      glm::dvec3(8.0, -1.5, 4.99))
{
    std::vector<Shape*> room = ShapeFactory::createRoom();
    shapes.insert(shapes.end(), room.begin(), room.end());

    std::vector<Shape*> cube = ShapeFactory::createCube(glm::dvec3(5, 3, 0), 2.0, Material(ColorDBL::blue(), Material::type::DIFFUSE));
    shapes.insert(shapes.end(), cube.begin(), cube.end());

    shapes.push_back(light.getLightShape());
}

Scene::~Scene() {
    for (Shape* shape : shapes) {
        delete shape;
    }
}

void Scene::render(int numThreads, int samplesPerPixel) {
    int totalPixels = camera.width * camera.height;
    std::atomic<int> processedPixels{0};
    std::atomic<int> progress{0};
    std::vector<ColorDBL> localPixels(camera.width * camera.height);
    std::mutex localPixelsMutex;
    std::mutex printMutex;

    auto modifyLocalPixels = [&](int index, const ColorDBL& color) {
        std::lock_guard<std::mutex> guard(localPixelsMutex);
        localPixels[index] = color;
    };

    auto updateProgress = [&]() {
        int newProgress = static_cast<int>((static_cast<double>(processedPixels) / totalPixels) * 100);
        if (newProgress > progress) {
            progress = newProgress;
            std::lock_guard<std::mutex> lock(printMutex);
            std::cout << "\rRendering scene: " << progress << "%" << std::flush;
        }
    };

    auto renderRange = [&](int start, int end) {
        for (int j = start; j < end; j++) {
            for (int i = 0; i < camera.width; i++) {
                ColorDBL color(0, 0, 0);
                for (int s = 0; s < samplesPerPixel; s++) {
                    Ray* ray = camera.getRay(i, j);
                    ray->traceRay(this);
                    ray->PixelRayColor(this);
                    color += ray->color;
                    delete ray; // free space
                }
                color /= samplesPerPixel;
                modifyLocalPixels(j * camera.width + i, color);
                processedPixels++;
                updateProgress();
            }
        }
    };

    int maxThreads = std::thread::hardware_concurrency();
    if (numThreads > maxThreads)
        numThreads = maxThreads;
        
    std::cout << "Rendering scene using " << numThreads << " thread(s) with " << samplesPerPixel << " samples\n";
    auto startTimer = std::chrono::high_resolution_clock::now();

    int chunkSize = camera.height / numThreads;
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; i++) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? camera.height : (i + 1) * chunkSize;
        threads.push_back(std::thread(renderRange, start, end));
    }

    for (std::thread& thread : threads) {
        thread.join();
    }

    pixels = std::move(localPixels);
    std::cout << "\nScene rendered successfully.\n";
    auto endTimer = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = endTimer - startTimer;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";
}

bool Scene::findNearestIntersection(Ray* ray, Shape*& hitShape, double &t_min) const {
    t_min = std::numeric_limits<double>::max();
    hitShape = nullptr;

    for (Shape* shape : this->shapes) {
        double t = shape->intersect(ray);
        if (t > 0.0 && t < t_min) {
            t_min = t;
            hitShape = shape;
        }
    }
    return hitShape != nullptr;
}

void Scene::saveImage(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    file << "P6\n" << camera.width << " " << camera.height << "\n255\n";
    int totalPixels = camera.width * camera.height;
    int processedPixels = 0;
    int progress = 0;
    std::cout << "Saving image";

    auto updateProgress = [&]() {
        int newProgress = static_cast<int>((static_cast<double>(processedPixels) / totalPixels) * 100);
        if (newProgress > progress) {
            progress = newProgress;
            std::cout << "\rSaving image";
            for (int i = 0; i < (progress % 5); ++i)
                std::cout << ".";
            std::cout << std::flush;
        }
    };

    for (const auto& rgb : pixels) {
        glm::dvec3 color = rgb.toRGB();
        file.put(static_cast<unsigned char>(glm::clamp(color.r, 0.0, 255.0)));
        file.put(static_cast<unsigned char>(glm::clamp(color.g, 0.0, 255.0)));
        file.put(static_cast<unsigned char>(glm::clamp(color.b, 0.0, 255.0)));
        processedPixels++;
        updateProgress();
    }
    file.close();
    std::cout << " to " << filename << std::endl;
}
