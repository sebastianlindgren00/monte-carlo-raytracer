#ifndef SHAPE_H
#define SHAPE_H

#include "ray.h"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>
#include "material.h"

class Shape {
public:
    Shape(Material material);
    virtual ~Shape();
    virtual double intersect(Ray* ray);
    
    virtual glm::dvec3 getNormal(glm::dvec3 hitPoint) const;
    virtual glm::dvec3 getNormal(glm::dvec3 hitPoint);
    Material getMaterial() const;
protected:
    Material material;
};

class Plane : public Shape {
public:
    Plane(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, Material material);

    double intersect(Ray* ray) override;
    glm::dvec3 getNormal();
    glm::dvec3 getNormal(glm::dvec3 hitPoint) override;
    
    glm::dvec3 bottomLeft, topLeft, bottomRight, topRight, normal;
};

class Sphere : public Shape {
public:
    Sphere(glm::dvec3 center, double radius, Material material);
    double intersect(Ray* ray) override;
    glm::dvec3 getNormal(glm::dvec3 hitPoint) override;
    
    glm::dvec3 center;
    double radius;
};

class Triangle : public Shape {
public:
    Triangle(glm::dvec3 top, glm::dvec3 baseLeft, glm::dvec3 baseRight, Material material);
    double intersect(Ray* ray) override;
    glm::dvec3 getNormal(glm::dvec3 hitPoint) override;
    
    glm::dvec3 top, baseLeft, baseRight, normal;
};

class ShapeFactory { 
public:
    static std::vector<Shape*> createShapes();
    static std::vector<Shape*> createRoom();
    static std::vector<Shape*> createCube(glm::dvec3 position, double size, Material material);
};

#endif // SHAPE_H
