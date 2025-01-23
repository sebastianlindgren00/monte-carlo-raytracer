#include "include/scene.h"
#include <fstream>
#include <filesystem>
namespace fs = std::filesystem;

int main () {
    int numThreads = std::thread::hardware_concurrency();
    int samplesPerPixel = 12;

    Scene scene;
    scene.render(numThreads, samplesPerPixel);
    
    fs::path root = fs::current_path().parent_path().parent_path();
    const std::string filePath = (root / "result.ppm").string();
    scene.saveImage(filePath);

    // Open the image in Visual Studio Code
    std::string command = "code \"" + filePath + "\"";
    system(command.c_str());

    return 0;
}