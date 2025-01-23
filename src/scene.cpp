#include "include/scene.h"
#include <thread>
#include <ppl.h>
#include "scene.h"

Scene::Scene() : camera(800, 800), light(glm::dvec3(5.0, 1.5, 4.90), glm::dvec3(5.0, -1.5, 4.90), glm::dvec3(8.0, 1.5, 4.90), glm::dvec3(8.0, -1.5, 4.90)) { 
    shapes = ShapeFactory::createShapes();
    shapes.push_back(light.getLightShape());
}

Scene::~Scene() {
    for (Shape* shape : shapes) {
        delete shape;
    }
}

// void Scene::renderRange(int start, int end) {
//     std::cout << "Rendering range: " << start << " - " << end << std::endl;
//     for (int j = start; j < end; j++) {
//         for (int i = 0; i < camera.width; i++) {
//             ColorDBL color(0, 0, 0);
//             int samplesPerPixel = 8;
//             for (int s = 0; s < samplesPerPixel; s++) {
//                 Ray* ray = camera.getRay(i, j);
//                 ray->traceRay(this, 0);
//                 color += PixelRayColor(ray);
//                 delete ray;
//             }
//             pixels[j * camera.width + i] = color / samplesPerPixel;
//         }
//     }
// }

void Scene::render(int numThreads = 1) {

    int totalPixels = camera.width * camera.height;
    std::atomic<int> processedPixels{0};
    std::atomic<int> progress{0};
    std::vector<ColorDBL> localPixels(camera.width * camera.height); // Preallocate the size
    std::mutex localPixelsMutex;
    std::mutex printMutex;

    // Function that modifies localPixels
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

    // Update renderRange to use modifyLocalPixels
    auto renderRange = [&](int start, int end) {
        std::cout << "Rendering range: " << start << " - " << end << "\n" << std::endl;
        for (int j = start; j < end; j++) {
            for (int i = 0; i < camera.width; i++) {
                ColorDBL color(0, 0, 0);
                int samplesPerPixel = 8;
                for (int s = 0; s < samplesPerPixel; s++) {
                    Ray* ray = camera.getRay(i, j);
                    ray->traceRay(this, 0);
                    color += PixelRayColor(ray);
                    delete ray;
                }
                color /= samplesPerPixel;
                modifyLocalPixels(j * camera.width + i, color);
                processedPixels++;
                updateProgress();
            }
        }
    };

    const int MAX_THREADS = std::thread::hardware_concurrency();
    if (numThreads > MAX_THREADS)
        numThreads = MAX_THREADS;
        
    std::cout << "Rendering scene using " << numThreads << " threads...\n";
    auto startTimer = std::chrono::high_resolution_clock::now();

    int chunkSize = camera.height / numThreads;
    std::vector<std::thread> threads;

    // Create threads
    for (int i = 0; i < numThreads; i++) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? camera.height : (i + 1) * chunkSize;
        threads.push_back(std::thread(renderRange, start, end));
    }

    // Join threads
    for (std::thread& thread : threads) {
        thread.join();
    }

    // concurrency::parallel_for((size_t)0, (size_t)camera.height, [&](size_t j) {
    //     for (int i = 0; i < camera.width; i++) {
    //         ColorDBL color(0, 0, 0);
    //         int samplesPerPixel = 8;
    //         for (int s = 0; s < samplesPerPixel; s++) {
    //             Ray* ray = camera.getRay(i, j);
    //             ray->traceRay(this, 0);
    //             color += PixelRayColor(ray);
    //             delete ray;
    //         }
    //         color /= samplesPerPixel;
    //         localPixels[j * camera.width + i] = color;
    //         processedPixels++;
    //         updateProgress();
    //     }
    // });

    // Replace `pixels` with `localPixels` at the end
    pixels = std::move(localPixels);

    std::cout << "\nScene rendered successfully.\n";
    auto endTimer = std::chrono::high_resolution_clock::now();
    std::cout << "Elapsed time: " << (endTimer - startTimer).count() << " seconds\n";
}

bool Scene::findNearestIntersection(Ray* ray, Shape*& hitShape, double& t_min) const{
    t_min = std::numeric_limits<double>::max();
    hitShape = nullptr;

    for (Shape* shape : this->shapes) {
        double t = shape->intersect(ray);
        if (t > 0.0 && t < t_min) {
            t_min = t;
            hitShape = shape;
        }
    }

    return hitShape != nullptr; // Return true if an intersection was found
}


ColorDBL Scene::PixelRayColor(Ray* ray) {
    ColorDBL color(0, 0, 0);
    while(ray->nextRay != nullptr){
        ray = ray->nextRay;
    }

    double t_min = 0.0;
    Shape* hitShape = nullptr;

    // First ray intersection
    if (findNearestIntersection(ray, hitShape, t_min)) {
        glm::dvec3 hitPoint = ray->pointAtSurface(t_min) + 0.001;
        glm::dvec3 normal = hitShape->getNormal(hitPoint);

        switch(hitShape->getMaterial().getMaterialType()){
            case Material::type::LIGHT: {
                return hitShape->getMaterial().getColor();
            }

            case Material::type::DIFFUSE: {
                ColorDBL diffuseColor = ray->computeIrradiance(this, hitPoint, hitShape, &light);
                color += diffuseColor;
                return color;
            }

            case Material::type::MIRROR: {
                Ray reflectedRay(hitPoint, glm::normalize(glm::reflect(ray->direction, normal)));
                reflectedRay.traceRay(this, 0);
                color += PixelRayColor(&reflectedRay);
                return color;
            }
        }   
    }
    else {
        return color;
    }

    // Continue tracing the ray

    while(ray->previousRay != nullptr){
        ray = ray->previousRay;
        double t_min = 0.0;
        Shape* hitShape = nullptr;

        if (findNearestIntersection(ray, hitShape, t_min)) {
            glm::dvec3 hitPoint = ray->pointAtSurface(t_min) + 0.001;
            glm::dvec3 normal = hitShape->getNormal(hitPoint);

            switch(hitShape->getMaterial().getMaterialType()){
                case Material::type::LIGHT: {
                    return hitShape->getMaterial().getColor();
                }

                case Material::type::DIFFUSE: {
                    ColorDBL diffuseColor = ray->computeIrradiance(this, hitPoint, hitShape, &light);
                    color += diffuseColor;
                    return color;
                }

                case Material::type::MIRROR: {
                    Ray reflectedRay(hitPoint, glm::normalize(glm::reflect(ray->direction, normal)));
                    reflectedRay.traceRay(this, 0);
                    color += PixelRayColor(&reflectedRay);
                    return color;
                }
            }   
        }

        else {
            return color;
        }
    }
}

void Scene::saveImage(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    file << "P6\n" << camera.width << " " << camera.height << "\n255\n";
    int totalPixels = camera.width * camera.height;
    int processedPixels = 0;
    int progress = 0;
    std::cout << "Saving image: 0%";

    auto updateProgress = [&]() {
        int newProgress = static_cast<int>((static_cast<double>(processedPixels) / totalPixels) * 100);
        if (newProgress > progress) {
            progress = newProgress;
            std::cout << "\rSaving image: " << progress << "%" << std::flush;
        }
    };

    for (const auto& rgb : pixels) {
        glm::dvec3 color = rgb.toRGB();
        file.put(static_cast<unsigned char>(glm::clamp((color.r), 0.0, 255.0)));
        file.put(static_cast<unsigned char>(glm::clamp((color.g), 0.0, 255.0)));
        file.put(static_cast<unsigned char>(glm::clamp((color.b), 0.0, 255.0)));
        processedPixels++;
        updateProgress();
    }
    file.close();
    std::cout << "\nImage saved successfully.\n";
}
