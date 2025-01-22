#include "include/camera.h"
#include "include/light.h"
#include "include/colorDBL.h"
#include "include/material.h"
#include "include/ray.h"
#include "include/shape.h"
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
    void render();// byt till parallell for (kör multi core, 800% increase :O)

    ColorDBL PixelRayColor(Ray* ray);

    void saveImage(const std::string& filename);

    bool findNearestIntersection(Ray* ray, Shape*& hitShape, double& t_min) const;

    Camera camera;
    std::vector<Shape*> shapes;
    std::vector<ColorDBL> pixels;
    Light light;
};