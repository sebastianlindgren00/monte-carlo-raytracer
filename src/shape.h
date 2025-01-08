#pragma once
#include "ray.h"
#include "glm/glm.hpp"
#include <iostream>
#include <vector>

class Shape {
public:
    Shape(glm::dvec3 color) : color(color) {}
    virtual ~Shape() {}
    virtual float intersect(const Ray& ray) = 0;
    
    virtual glm::dvec3 getNormal(const glm::dvec3& hitPoint) {
        return glm::dvec3(0.0f, 0.0f, 0.0f);
    }

    const std::string type;
    glm::dvec3 color;
};

class DiffusePlane : public Shape {
    public:
    DiffusePlane(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, glm::dvec3 color) : Shape(color), bottomLeft(bottomLeft), topLeft(topLeft), bottomRight(bottomRight), topRight(topRight) {}

    float intersect(const Ray& ray) override {
        glm::dvec3 c1 = bottomRight - bottomLeft;
        glm::dvec3 c2 = topLeft - bottomLeft;
        glm::dvec3 normal = glm::normalize(glm::cross(c1, c2));
        float denominator = glm::dot(normal, ray.direction);
        if (std::abs(denominator) < 1e-6f) return -1; // Ray is parallel to the plane
        float t = glm::dot((bottomLeft - ray.origin), normal) / denominator;

            if (t >= 0.0f) {

                glm::dvec3 intersectionPoint = ray.origin + static_cast<double>(t) * ray.direction;

                float a = glm::dot((intersectionPoint - bottomLeft), c1) / glm::dot(c1, c1);
                float b = glm::dot((intersectionPoint - bottomLeft), c2) / glm::dot(c2, c2);

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
    
    glm::dvec3 getNormal(const glm::dvec3& hitPoint) override {
        return glm::normalize(glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft));
    }

    glm::dvec3 bottomLeft;
    glm::dvec3 topLeft;
    glm::dvec3 bottomRight;
    glm::dvec3 topRight;
    float distance;
    const std::string type = "DIFFUSE";
};

class SpecularPlane: public Shape {

    public:
    SpecularPlane(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, glm::dvec3 color) : Shape(color), bottomLeft(bottomLeft), topLeft(topLeft), bottomRight(bottomRight), topRight(topRight) {}

    float intersect(const Ray& ray) override {
        glm::dvec3 c1 = bottomRight - bottomLeft;
        glm::dvec3 c2 = topLeft - bottomLeft;
        glm::dvec3 normal = glm::normalize(glm::cross(c1, c2));
        float denominator = glm::dot(normal, ray.direction);
        if (std::abs(denominator) < 1e-6f) return -1; // Ray is parallel to the plane
        float t = glm::dot((bottomLeft - ray.origin), normal) / denominator;

            if (t >= 0.0f) {

                glm::dvec3 intersectionPoint = ray.origin + static_cast<double>(t) * ray.direction;

                float a = glm::dot((intersectionPoint - bottomLeft), c1) / glm::dot(c1, c1);
                float b = glm::dot((intersectionPoint - bottomLeft), c2) / glm::dot(c2, c2);

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
    
    glm::dvec3 getNormal(const glm::dvec3& hitPoint) override {
        return glm::normalize(glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft));
    }

    glm::dvec3 bottomLeft;
    glm::dvec3 topLeft;
    glm::dvec3 bottomRight;
    glm::dvec3 topRight;
    float distance;
};

class ReflectivePlane: public Shape {

    public:
    ReflectivePlane(glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, glm::dvec3 color) : Shape(color), bottomLeft(bottomLeft), topLeft(topLeft), bottomRight(bottomRight), topRight(topRight) {}

    float intersect(const Ray& ray) override {
        glm::dvec3 c1 = bottomRight - bottomLeft;
        glm::dvec3 c2 = topLeft - bottomLeft;
        glm::dvec3 normal = glm::normalize(glm::cross(c1, c2));
        float denominator = glm::dot(normal, ray.direction);
        if (std::abs(denominator) < 1e-6f) return -1; // Ray is parallel to the plane
        float t = glm::dot((bottomLeft - ray.origin), normal) / denominator;

            if (t >= 0.0f) {

                glm::dvec3 intersectionPoint = ray.origin + static_cast<double>(t) * ray.direction;

                float a = glm::dot((intersectionPoint - bottomLeft), c1) / glm::dot(c1, c1);
                float b = glm::dot((intersectionPoint - bottomLeft), c2) / glm::dot(c2, c2);

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
    
    glm::dvec3 getNormal(const glm::dvec3& hitPoint) override {
        return glm::normalize(glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft));
    }

    glm::dvec3 bottomLeft;
    glm::dvec3 topLeft;
    glm::dvec3 bottomRight;
    glm::dvec3 topRight;
    const std::string type = "MIRROR";
    float distance;
};

class DiffuseSphere : public Shape {
public:
    DiffuseSphere(glm::dvec3 center, float radius, glm::dvec3 color) : Shape(color), center(center), radius(radius) {}

    float intersect(const Ray& ray) override { 
        glm::dvec3 oc = ray.origin - center;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0) {
            return -1;
        }
        return (-b - sqrt(discriminant)) / (2.0f * a);
    }

    glm::dvec3 getNormal(const glm::dvec3& hitPoint) override {
        return glm::normalize(hitPoint - center);
    }

    glm::dvec3 center;
    const std::string type = "DIFFUSE";
    float radius;
};

class SpecularSphere : public Shape {
public:
    SpecularSphere(glm::dvec3 center, float radius, glm::dvec3 color) : Shape(color), center(center), radius(radius) {}

    float intersect(const Ray& ray) override { 
        glm::dvec3 oc = ray.origin - center;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0) {
            return -1;
        }
        return (-b - sqrt(discriminant)) / (2.0f * a);
    }

    glm::dvec3 getNormal(const glm::dvec3& hitPoint) override {
        return glm::normalize(hitPoint - center);
    }

    glm::dvec3 center;
    float radius;
};

class ReflectiveSphere : public Shape {
public:
    ReflectiveSphere(glm::dvec3 center, float radius, glm::dvec3 color) : Shape(color), center(center), radius(radius) {}

    float intersect(const Ray& ray) override { 
        glm::dvec3 oc = ray.origin - center;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0) {
            return -1;
        }
        return (-b - sqrt(discriminant)) / (2.0f * a);
    }

    glm::dvec3 getNormal(const glm::dvec3& hitPoint) override {
        return glm::normalize(hitPoint - center);
    }

    glm::dvec3 center;
    float radius;
};

class DiffuseTriangle : public Shape {
public:
    DiffuseTriangle(glm::dvec3 top, glm::dvec3 baseLeft, glm::dvec3 baseRight, glm::dvec3 color) 
        : Shape(color), top(top), baseLeft(baseLeft), baseRight(baseRight) {}

    // Möller–Trumbore Intersection Algorithm
    float intersect(const Ray& ray) override {
        // Compute the edges of the triangle
        glm::dvec3 edge1 = baseLeft - top;
        glm::dvec3 edge2 = baseRight - top;

        // Compute the cross product between the ray's direction and edge2
        glm::dvec3 h = glm::cross(ray.direction, edge2);
        
        // Compute the determinant (a)
        float a = glm::dot(edge1, h);
        if (a > -1e-6 && a < 1e-6) {
            return -1; // Ray is parallel to the triangle
        }

        // Compute the inverse of a
        float f = 1 / a;

        // Compute the vector from the ray origin to the triangle vertex top
        glm::dvec3 s = ray.origin - top;

        // Calculate barycentric coordinate u
        float u = f * glm::dot(s, h);
        if (u < 0 || u > 1) {
            return -1; // Intersection is outside the triangle
        }

        // Compute the cross product of s and edge1
        glm::dvec3 q = glm::cross(s, edge1);

        // Calculate barycentric coordinate v
        float v = f * glm::dot(ray.direction, q);
        if (v < 0 || u + v > 1) {
            return -1; // Intersection is outside the triangle
        }

        // Calculate the distance from the ray origin to the intersection point
        float t = f * glm::dot(edge2, q);
        if (t > 1e-6) {
            return t; // Ray intersects the triangle
        }

        return -1; // No intersection
    }

    // Compute the normal of the triangle (cross product of two edges)
    glm::dvec3 getNormal(const glm::dvec3& hitPoint) override {
        return glm::normalize(glm::cross(baseLeft - top, baseRight - top));
    }

    const std::string type = "DIFFUSE";
    
    glm::dvec3 top, baseLeft, baseRight;
};

class SpecularTriangle : public Shape {
public:
    SpecularTriangle(glm::dvec3 top, glm::dvec3 baseLeft, glm::dvec3 baseRight, glm::dvec3 color) : Shape(color), top(top), baseLeft(baseLeft), baseRight(baseRight) {}

    float intersect(const Ray& ray) override {
        glm::dvec3 edge1 = baseLeft - top;
        glm::dvec3 edge2 = baseRight - top;
        glm::dvec3 h = glm::cross(ray.direction, edge2);
        float a = glm::dot(edge1, h);
        if (a > -1e-6 && a < 1e-6) {
            return -1;
        }
        float f = 1 / a;
        glm::dvec3 s = ray.origin - top;
        float u = f * glm::dot(s, h);
        if (u < 0 || u > 1) {
            return -1;
        }
        glm::dvec3 q = glm::cross(s, edge1);
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

    glm::dvec3 getNormal(const glm::dvec3& hitPoint) override {
        return glm::normalize(glm::cross(baseLeft - top, baseRight - top));
    }

    glm::dvec3 top;
    glm::dvec3 baseLeft;
    glm::dvec3 baseRight;
};

class ReflectiveTriangle : public Shape {
public:
    ReflectiveTriangle(glm::dvec3 top, glm::dvec3 baseLeft, glm::dvec3 baseRight, glm::dvec3 color) : Shape(color), top(top), baseLeft(baseLeft), baseRight(baseRight) {}

    float intersect(const Ray& ray) override {
        glm::dvec3 edge1 = baseLeft - top;
        glm::dvec3 edge2 = baseRight - top;
        glm::dvec3 h = glm::cross(ray.direction, edge2);
        float a = glm::dot(edge1, h);
        if (a > -1e-6 && a < 1e-6) {
            return -1;
        }
        float f = 1 / a;
        glm::dvec3 s = ray.origin - top;
        float u = f * glm::dot(s, h);
        if (u < 0 || u > 1) {
            return -1;
        }
        glm::dvec3 q = glm::cross(s, edge1);
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

    glm::dvec3 getNormal(const glm::dvec3& hitPoint) override {
        return glm::normalize(glm::cross(baseLeft - top, baseRight - top));
    }

    glm::dvec3 top;
    glm::dvec3 baseLeft;
    glm::dvec3 baseRight;
};

class ShapeFactory { 
public:
    static std::vector<Shape*> createShapes() {
        std::vector<Shape*> shapes = createRoom();
        //shapes = createFloatingObjects(shapes);
        return shapes;
    }
private:
    static std::vector<Shape*> createRoom() {
        std::vector<Shape*> shapes;
        // Walls
        shapes.push_back(new DiffusePlane(glm::dvec3(10, 6, 5), glm::dvec3(13, 0, 5), glm::dvec3(10, 6, -5), glm::dvec3(13, 0, -5), glm::dvec3(1.0f, 0.0f, 0.0f))); // Far right
        shapes.push_back(new DiffusePlane(glm::dvec3(13, 0, 5), glm::dvec3(10, -6, 5), glm::dvec3(13, 0, -5), glm::dvec3(10, -6, -5), glm::dvec3(0.0f, 1.0f, 0.0f))); // Far left
        shapes.push_back(new DiffusePlane(glm::dvec3(10, -6, 5), glm::dvec3(0, -6, 5), glm::dvec3(10, -6, -5), glm::dvec3(0, -6, -5), glm::dvec3(0.0f, 0.0f, 1.0f))); // Left
        shapes.push_back(new DiffusePlane(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), glm::dvec3(1.0f, 0.0f, 0.0f)));
        shapes.push_back(new DiffusePlane(glm::dvec3(-3, 0, 5), glm::dvec3(0, 6, 5), glm::dvec3(-3, 0, -5), glm::dvec3(0, 6, -5), glm::dvec3(0.0f, 1.0f, 0.0f)));
        shapes.push_back(new DiffusePlane(glm::dvec3(0, -6, 5), glm::dvec3(-3, 0, 5), glm::dvec3(0, -6, -5), glm::dvec3(-3, 0, -5), glm::dvec3(0.0f, 0.0f, 1.0f)));
        shapes.push_back(new DiffusePlane(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(0, -6, 5), glm::dvec3(10, -6, 5), glm::dvec3(1, 0.1, 0.1)));

        //Floor
        shapes.push_back(new DiffuseTriangle(glm::dvec3(0, 0, 5), glm::dvec3(10, 0, 5), glm::dvec3(0, 0, -5), glm::dvec3(1.0f, 1.0f, 1.0f)));
        shapes.push_back(new DiffuseTriangle(glm::dvec3(10, 0, 5), glm::dvec3(10, 0, -5), glm::dvec3(0, 0, -5), glm::dvec3(1.0f, 1.0f, 1.0f)));
        shapes.push_back(new DiffusePlane(glm::dvec3(0, 6, -5), glm::dvec3(10, 6, -5), glm::dvec3(0, -6, -5), glm::dvec3(10, -6, -5), glm::dvec3(0.5f, 0.5f, 0.5f)));

        // Roof
        shapes.push_back(new DiffuseTriangle(glm::dvec3(-3.0, 0.0, -5.0), glm::dvec3(0.0, -6.0, -5.0), glm::dvec3(0, 6.0, -5.0), glm::dvec3(0.5f, 0.5f, 0.5f)));
        shapes.push_back(new DiffuseTriangle(glm::dvec3(10, 0, -5.0), glm::dvec3(10, 0, -5.0), glm::dvec3(0, 0, -5.0), glm::dvec3(0.5f, 0.5f, 0.5f)));
        //shapes.push_back(new DiffusePlane(glm::dvec3(0, 6, 5), glm::dvec3(10, 6, 5), glm::dvec3(0, -6, 5), glm::dvec3(10, -6, 5), glm::dvec3(0.7f, 0.7f, 0.7f))); // somehow roof, but it's -5 on z
        return shapes;
    }
    static std::vector<Shape*> createFloatingObjects(std::vector<Shape*> shapes) {
        shapes.push_back(new DiffuseSphere(glm::dvec3(0, 0, 0), 1.0f, glm::dvec3(1.0f, 0.0f, 0.0f)));
        shapes.push_back(new DiffuseSphere(glm::dvec3(0, 0, 0), 1.0f, glm::dvec3(0.0f, 1.0f, 0.0f)));
        shapes.push_back(new DiffuseSphere(glm::dvec3(0, 0, 0), 1.0f, glm::dvec3(0.0f, 0.0f, 1.0f)));
        return shapes;
    }
};