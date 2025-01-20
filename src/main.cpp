#include "include/scene.h"
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

int main () {
    Scene scene;
    scene.render();
    
    fs::path root = fs::current_path().parent_path().parent_path();
    const std::string filePath = (root / "result.ppm").string();
    scene.saveImage(filePath);
    std::cout << "Image saved to " << filePath << std::endl;

    system(("code " + filePath).c_str());

    return 0;
}