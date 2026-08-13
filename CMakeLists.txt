cmake_minimum_required(VERSION 3.20)

set(CMAKE_GENERATOR_PLATFORM x64)

project(CursoOpenGL LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(CursoOpenGL
    src/main.cpp

    src/camera/Camera.cpp

    src/graphics/EBO.cpp
    src/graphics/VAO.cpp
    src/graphics/VBO.cpp

    src/shader/shaderClass.cpp

    src/textures/Texture.cpp
    src/textures/stb.cpp

    external/src/glad.c
)

target_include_directories(CursoOpenGL PRIVATE
    Libraries/include

    src/camera
    src/graphics
    src/shader
    src/textures
)

target_link_libraries(CursoOpenGL PRIVATE
    "${CMAKE_SOURCE_DIR}/Libraries/lib/glfw3.lib"
    opengl32
)