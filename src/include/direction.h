

class LocalDirection {
public:
    LocalDirection(double theta, double pi) : theta(theta), phi(phi) {}

    static LocalDirection GetRandomLocalDirection() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dis(0, 1);

        double u = dis(gen);
        double v = dis(gen);

        double theta = 2 * M_PI * u;
        double phi = acos(2 * v - 1);

        return LocalDirection(theta, phi);
    }

    static glm::dvec3 HemisphericalToCartesian(const LocalDirection& localDir) {
        return glm::dvec3(
            sin(localDir.phi) * cos(localDir.theta),
            sin(localDir.phi) * sin(localDir.theta),
            cos(localDir.phi)
        );
    }

    static glm::dvec3 LocalCartesianToWorldCartesian(const glm::dvec3& localDir, const glm::dvec3& normal) {
        glm::dvec3 tangent = glm::normalize(glm::cross(normal, glm::dvec3(0.0, 0.0, 1.0)));
        glm::dvec3 bitangent = glm::normalize(glm::cross(normal, tangent));

        return localDir.x * tangent + localDir.y * bitangent + localDir.z * normal;
    }

    double theta;
    double phi;
};