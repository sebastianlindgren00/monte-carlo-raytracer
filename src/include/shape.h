#ifndef SHAPE_H
#define SHAPE_H

#include "include/ray.h"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>

class Shape {
public:
    Shape(Material material);
    virtual ~Shape();
    virtual double intersect(Ray* ray);
    
    glm::dvec3 getNormal();
    glm::dvec3 getNormal() const;
    virtual glm::dvec3 getNormal(glm::dvec3 hitPoint);
    virtual glm::dvec3 getNormal(glm::dvec3 hitPoint) const;

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

    // Möller–Trumbore Intersection Algorithm
    double intersect(Ray* ray) override;

    // Compute the normal of the triangle (cross product of two edges)
    glm::dvec3 getNormal(glm::dvec3 hitPoint) override;
    
    glm::dvec3 top, baseLeft, baseRight, normal;
};

class ShapeFactory { 
public:
    static std::vector<Shape*> createShapes();
private:
    static std::vector<Shape*> createRoom();
};

#endif // SHAPE_H