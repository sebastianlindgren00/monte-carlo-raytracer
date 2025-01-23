#include "include/shape.h"

Shape::Shape(Material material) : material(material) {}
Shape::~Shape() {}

double Shape::intersect(Ray* ray) {
    return -1.0;
}

glm::dvec3 Shape::getNormal(glm::dvec3 hitPoint) const {
    return glm::dvec3(0.0, 0.0, 0.0);
}
glm::dvec3 Shape::getNormal(glm::dvec3 hitPoint) {
    return glm::dvec3(0.0, 0.0, 0.0);
}

Material Shape::getMaterial() const {
    return material;
}

#pragma region Plane
Plane::Plane(
    glm::dvec3 topLeft, glm::dvec3 topRight, glm::dvec3 bottomLeft, glm::dvec3 bottomRight, Material material) : 
    Shape(material), bottomLeft(bottomLeft), topLeft(topLeft), bottomRight(bottomRight), topRight(topRight), 
        normal(glm::normalize(glm::cross(bottomRight - bottomLeft, topLeft - bottomLeft))
) {}

double Plane::intersect(Ray* ray) {
    glm::dvec3 c1 = bottomRight - bottomLeft;
    glm::dvec3 c2 = topLeft - bottomLeft;
    glm::dvec3 normal = glm::normalize(glm::cross(c1, c2));
    double denominator = glm::dot(normal, ray->direction);
    if (std::abs(denominator) < 1e-6f) return -1.0; // Ray is parallel to the plane
    double t = glm::dot((bottomLeft - ray->origin), normal) / denominator;

    glm::dvec3 intersectionPoint = ray->origin + static_cast<double>(t) * ray->direction;

    double a = glm::dot((intersectionPoint - bottomLeft), c1) / glm::dot(c1, c1);
    double b = glm::dot((intersectionPoint - bottomLeft), c2) / glm::dot(c2, c2);

    if (a >= 0.0 && a <= 1.0 && b >= 0.0 && b <= 1.0) {
        return t;
    }
    else {
        return -1.0;
    }
}

glm::dvec3 Plane::getNormal() {
    return normal;
}

glm::dvec3 Plane::getNormal(glm::dvec3 hitPoint) {
    return normal;
}
#pragma endregion

#pragma region Sphere
Sphere::Sphere(glm::dvec3 center, double radius, Material material) : Shape(material), center(center), radius(radius) {}

double Sphere::intersect(Ray* ray) {
    auto solveQuadratic = [&](const double &a, const double &b, const double &c, double &x0, double &x1) {
        double discriminant = b * b - 4 * a * c;
        if (discriminant < 0) return false;
        else if (discriminant == 0) x0 = x1 = -0.5 * b / a;
        else {
            double q = (b > 0) ?
                -0.5 * (b + sqrt(discriminant)) :
                -0.5 * (b - sqrt(discriminant));
            x0 = q / a;
            x1 = c / q;
        }
        if (x0 > x1) std::swap(x0, x1);
        
        return true;
    };

    glm::dvec3 oc = ray->origin - center;
    double a = glm::dot(ray->direction, ray->direction);
    double b = 2.0 * glm::dot(oc, ray->direction);
    double c = glm::dot(oc, oc) - radius * radius;
    double t0, t1;
    if (!solveQuadratic(a, b, c, t0, t1)) return -1.0;

    if (t0 > t1) std::swap(t0, t1);
    if (t0 < 0) {
        t0 = t1;
        if (t0 < 0) 
            return -1.0;
    }

    return t0;
}

glm::dvec3 Sphere::getNormal(glm::dvec3 hitPoint) {
    glm::dvec3 normal = glm::normalize(hitPoint - center);
    return normal;
}
#pragma endregion

#pragma region Triangle
Triangle::Triangle(
    glm::dvec3 top, glm::dvec3 baseLeft, glm::dvec3 baseRight, Material material) : 
    Shape(material), top(top), baseLeft(baseLeft), baseRight(baseRight), 
        normal(glm::normalize(glm::cross(baseLeft - top, baseRight - top))
) {}

double Triangle::intersect(Ray* ray) {
    // Möller–Trumbore Intersection Algorithm
    glm::dvec3 D = ray->direction;
    glm::dvec3 edge1 = baseLeft - top;
    glm::dvec3 edge2 = baseRight - top;

    glm::dvec3 T = ray->origin - top; 
    glm::dvec3 P = glm::cross(D, edge2);
    glm::dvec3 Q = glm::cross(T, edge1);
    
    double t = glm::dot(Q, edge2) / glm::dot(P, edge1);
    double u = glm::dot(P, T) / glm::dot(P, edge1);
    double v = glm::dot(Q, D) / glm::dot(P, edge1);

    if (u >= 0.0 && v >= 0.0 && u + v < 1.0) {
        return t;
    }

    return -1; // No intersection
}

// Compute the normal of the triangle (cross product of two edges)
glm::dvec3 Triangle::getNormal(glm::dvec3 hitPoint) {
    return normal;
}
#pragma endregion

//ShapeFactory
std::vector<Shape*> ShapeFactory::createShapes() {
    std::vector<Shape *> shapes;
    
    std::vector<Shape*> room = createRoom();
    shapes.insert(shapes.end(), room.begin(), room.end());

    std::vector<Shape*> cube = createCube(glm::dvec3(5, 3, 0), 2.0, Material(ColorDBL::blue(), Material::type::DIFFUSE));
    shapes.insert(shapes.end(), cube.begin(), cube.end());

    Shape* sphere = new Sphere(glm::dvec3(6, 0, 0), 1.0, Material(ColorDBL::white(), Material::type::MIRROR));
    shapes.push_back(sphere);

    return shapes;
}

std::vector<Shape*> ShapeFactory::createRoom() {
    std::vector<Shape*> shapes;
    // Walls
    shapes.push_back(new Plane(glm::dvec3(10.0, 6.0, 5.0), glm::dvec3(13.0, 0.0, 5.0), glm::dvec3(10.0, 6.0, -5.0), glm::dvec3(13.0, 0.0, -5.0), Material(ColorDBL::red(), Material::type::DIFFUSE))); // Far right
    shapes.push_back(new Plane(glm::dvec3(13.0, 0.0, 5.0), glm::dvec3(10.0, -6.0, 5.0), glm::dvec3(13.0, 0.0, -5.0), glm::dvec3(10.0, -6.0, -5.0), Material(ColorDBL::green(), Material::type::DIFFUSE))); // Far left
    shapes.push_back(new Plane(glm::dvec3(10.0, -6.0, 5.0), glm::dvec3(0.0, -6.0, 5.0), glm::dvec3(10.0, -6, -5.0), glm::dvec3(0.0, -6.0, -5.0), Material(ColorDBL::blue(), Material::type::DIFFUSE))); // Left
    shapes.push_back(new Plane(glm::dvec3(0.0, 6.0, 5.0), glm::dvec3(10.0, 6.0, 5.0), glm::dvec3(0.0, 6.0, -5.0), glm::dvec3(10.0, 6.0, -5.0), Material(ColorDBL::red(), Material::type::DIFFUSE)));
    shapes.push_back(new Plane(glm::dvec3(-3.0, 0.0, 5.0), glm::dvec3(0.0, 6.0, 5.0), glm::dvec3(-3.0, 0.0, -5.0), glm::dvec3(0.0, 6.0, -5.0), Material(ColorDBL::green(), Material::type::DIFFUSE)));
    shapes.push_back(new Plane(glm::dvec3(0.0, -6.0, 5.0), glm::dvec3(-3.0, 0.0, 5.0), glm::dvec3(0.0, -6.0, -5.0), glm::dvec3(-3.0, 0.0, -5.0), Material(ColorDBL::blue(), Material::type::DIFFUSE)));

    //Floor
    shapes.push_back(new Triangle(glm::dvec3(13, 0, -5.0), glm::dvec3(10, 6, -5.0), glm::dvec3(10, -6, -5.0), Material(ColorDBL::grey(), Material::type::DIFFUSE)));
    shapes.push_back(new Plane(glm::dvec3(0, 6, -5.0), glm::dvec3(10, 6, -5.0), glm::dvec3(0, -6, -5.0), glm::dvec3(10, -6, -5.0), Material(ColorDBL::grey(), Material::type::DIFFUSE)));

    // Roof
    shapes.push_back(new Triangle(glm::dvec3(-3.0, 0.0, -5.0), glm::dvec3(0.0, -6.0, -5.0), glm::dvec3(0, 6.0, -5.0), Material(ColorDBL::blue(), Material::type::DIFFUSE)));
    shapes.push_back(new Triangle(glm::dvec3(10.0, 0.0, -5.0), glm::dvec3(10.0, 0.0, -5.0), glm::dvec3(0.0, 0.0, -5.0), Material(ColorDBL::blue(), Material::type::DIFFUSE)));
    shapes.push_back(new Plane(glm::dvec3(0.0, 6.0, 5.0), glm::dvec3(10.0, 6.0, 5.0), glm::dvec3(0.0, -6.0, 5.0), glm::dvec3(10.0, -6.0, 5.0), Material(ColorDBL::grey(), Material::type::DIFFUSE))); // somehow roof, but it's -5 on z

    return shapes;
}

std::vector<Shape*> ShapeFactory::createCube(glm::dvec3 position, double size, Material material) {
    std::vector<Shape*> shapes;
    double halfSize = size / 2.0;
    glm::dvec3 topLeftFront = glm::dvec3(position.x - halfSize, position.y + halfSize, position.z + halfSize);
    glm::dvec3 topRightFront = glm::dvec3(position.x + halfSize, position.y + halfSize, position.z + halfSize);
    glm::dvec3 bottomLeftFront = glm::dvec3(position.x - halfSize, position.y - halfSize, position.z + halfSize);
    glm::dvec3 bottomRightFront = glm::dvec3(position.x + halfSize, position.y - halfSize, position.z + halfSize);
    glm::dvec3 topLeftBack = glm::dvec3(position.x - halfSize, position.y + halfSize, position.z - halfSize);
    glm::dvec3 topRightBack = glm::dvec3(position.x + halfSize, position.y + halfSize, position.z - halfSize);
    glm::dvec3 bottomLeftBack = glm::dvec3(position.x - halfSize, position.y - halfSize, position.z - halfSize);
    glm::dvec3 bottomRightBack = glm::dvec3(position.x + halfSize, position.y - halfSize, position.z - halfSize);

    // Front
    shapes.push_back(new Plane(topLeftFront, topRightFront, bottomLeftFront, bottomRightFront, material));
    // Back
    shapes.push_back(new Plane(topRightBack, topLeftBack, bottomRightBack, bottomLeftBack, material));
    // Left
    shapes.push_back(new Plane(topLeftBack, topLeftFront, bottomLeftBack, bottomLeftFront, material));
    // Right
    shapes.push_back(new Plane(topRightFront, topRightBack, bottomRightFront, bottomRightBack, material));
    // Top
    shapes.push_back(new Plane(topLeftBack, topRightBack, topLeftFront, topRightFront, material));
    // Bottom
    shapes.push_back(new Plane(bottomLeftFront, bottomRightFront, bottomLeftBack, bottomRightBack, material));

    return shapes;
}