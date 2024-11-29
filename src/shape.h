#pragma once
#include "ray.h"
#include "glm/glm.hpp"
#include <iostream>

class Shape {
public:
    Shape(glm::vec3 color) : color(color) {}
    virtual ~Shape() {}
    virtual float intersect(const Ray& ray) = 0;
    
    virtual glm::vec3 getNormal(const glm::vec3& hitPoint) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    glm::vec3 color;
};


class Sphere : public Shape {
public:
    Sphere(glm::vec3 center, float radius, glm::vec3 color) : Shape(color), center(center), radius(radius) {}

    float intersect(const Ray& ray) override { 
        glm::vec3 oc = ray.origin - center;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0) {
            return -1;
        }
        return (-b - sqrt(discriminant)) / (2.0f * a);
    }

    glm::vec3 getNormal(const glm::vec3& hitPoint) override {
        return glm::normalize(hitPoint - center);
    }

    glm::vec3 center;
    float radius;
};

class Plane : public Shape {
public:
    Plane(glm::vec3 topLeft, glm::vec3 topRight, glm::vec3 bottomLeft, glm::vec3 bottomRight, glm::vec3 color) : Shape(color), distance(distance), bottomLeft(bottomLeft), topLeft(topLeft), bottomRight(bottomRight), topRight(topRight) {}

    float intersect(const Ray& ray) override {
        glm::vec3 c1 = bottomRight - bottomLeft;
        glm::vec3 c2 = topLeft - bottomLeft;
        glm::vec3 normal = glm::normalize(glm::cross(c1, c2));
        float denominator = glm::dot(normal, ray.direction);
        if (std::abs(denominator) < 1e-6f) return -1; // Ray is parallel to the plane
        float t = glm::dot((distance - ray.origin), normal) / denominator;

            if (t >= 0.0f) {

                glm::vec3 intersectionPoint = ray.origin + t * ray.direction;

                float a = glm::dot((intersectionPoint - distance), c1) / glm::dot(c1, c1);
                float b = glm::dot((intersectionPoint - distance), c2) / glm::dot(c2, c2);

                if (a >= 0.0f && a <= 1.0f && b >= 0.0f && b <= 1.0f) {
                    return t;
                }
                else {
                    return -1;
                }
            }
            else {
                return -1;
            }
    }
    
    glm::vec3 getNormal(const glm::vec3& hitPoint) override {
        return glm::normalize(glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft));
    }

    glm::vec3 bottomLeft;
    glm::vec3 topLeft;
    glm::vec3 bottomRight;
    glm::vec3 topRight;
    float distance;
};

class Triangle : public Shape {
public:
    Triangle(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 color) : Shape(color), v0(v0), v1(v1), v2(v2) {}

    float intersect(const Ray& ray) override {
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 h = glm::cross(ray.direction, edge2);
        float a = glm::dot(edge1, h);
        if (a > -1e-6 && a < 1e-6) {
            return -1;
        }
        float f = 1 / a;
        glm::vec3 s = ray.origin - v0;
        float u = f * glm::dot(s, h);
        if (u < 0 || u > 1) {
            return -1;
        }
        glm::vec3 q = glm::cross(s, edge1);
        float v = f * glm::dot(ray.direction, q);
        if (v < 0 || u + v > 1) {
            return -1;
        }
        float t = f * glm::dot(edge2, q);
        if (t > 1e-6) {
            return t;
        }
        return -1;
    }

    glm::vec3 getNormal(const glm::vec3& hitPoint) override {
        return glm::normalize(glm::cross(v1 - v0, v2 - v0));
    }

    glm::vec3 v0;
    glm::vec3 v1;
    glm::vec3 v2;
};

class ShapeFactory { 
public:
    static std::vector<Shape*> createShapes() {
        std::vector<Shape*> shapes;
        shapes.push_back(new Sphere(glm::vec3(0.0f, -1.0f, -0.8f), 1.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
        shapes.push_back(new Plane(glm::vec3(10, 6, 5), glm::vec3(13, 0, 5), glm::vec3(10, 6, -5), glm::vec3(13, 0, -5), glm::vec3(1.0f, 0.0f, 0.0f)));
        shapes.push_back(new Plane(glm::vec3(13, 0, 5), glm::vec3(10, -6, 5), glm::vec3(13, 0, -5), glm::vec3(10, -6, -5), glm::vec3(1.0f, 0.0f, 0.0f)));
        shapes.push_back(new Plane(glm::vec3(10, -6, 5), glm::vec3(0, -6, 5), glm::vec3(10, -6, -5), glm::vec3(0, -6, -5), glm::vec3(1.0f, 0.0f, 0.0f)));
        shapes.push_back(new Plane(glm::vec3(0, 6, 5), glm::vec3(10, 6, 5), glm::vec3(0, 6, -5), glm::vec3(10, 6, -5), glm::vec3(1.0f, 0.0f, 0.0f)));
        shapes.push_back(new Plane(glm::vec3(-3, 0, 5), glm::vec3(0, 6, 5), glm::vec3(-3, 0, -5), glm::vec3(0, 6, -5), glm::vec3(1.0f, 0.0f, 0.0f)));
        shapes.push_back(new Plane(glm::vec3(0, -6, 5), glm::vec3(-3, 0, 5), glm::vec3(0, -6, -5), glm::vec3(-3, 0, -5), glm::vec3(1.0f, 0.0f, 0.0f)));
        return shapes;
    }
};