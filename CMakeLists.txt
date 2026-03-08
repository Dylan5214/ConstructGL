cmake_minimum_required(VERSION 3.16)
project(ConstructGL VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Collect all .cpp files under src/
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(ConstructGL ${SOURCES})

target_include_directories(ConstructGL PRIVATE
    include
    ${OPENGL_INCLUDE_DIR}
)

target_link_libraries(ConstructGL PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
    glm::glm
)