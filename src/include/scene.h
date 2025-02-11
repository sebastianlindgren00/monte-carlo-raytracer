#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "light.h"
#include "colorDBL.h"
#include "material.h"
#include "ray.h"
#include "shape.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <random>
#include <ppl.h>

class Scene {
public:
    Scene();
    ~Scene();
    
    void renderRange(int start, int end);
    void render(int numThreads, int samplesPerPixel);
    void saveImage(const std::string& filename);
    bool findNearestIntersection(Ray* ray, Shape*& hitShape, double &t_min) const;

    Camera camera;
    std::vector<Shape*> shapes;
    std::vector<ColorDBL> pixels;
    Light light;
};

#endif // SCENE_H
