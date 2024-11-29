cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MonteCarloRaytracer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the source directory variable for convenience
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Collect all source files (add new .cpp files automatically)
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)

# Add the executable target
add_executable(MonteCarloRaytracer ${SOURCES})

# Include directories
target_include_directories(MonteCarloRaytracer PRIVATE ${SRC_DIR} ${SRC_DIR}/include)

# Link libraries (if needed in the future)
# target_link_libraries(MonteCarloRaytracer PRIVATE <library_name>)
