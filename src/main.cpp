#include "include/scene.h"
#include <fstream>

int main () {
    Scene scene;
    scene.render();

    const std::string filePath = "E:/Git/monte-carlo-raytracer/src/image/result.ppm";
    scene.saveImage(filePath);
    std::cout << "Image saved to " << filePath << std::endl;

    system(("code " + filePath).c_str());

    return 0;
}