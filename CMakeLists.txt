cmake_minimum_required(VERSION 3.20)

project(topoOptim
    VERSION 0.1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)

add_executable(topo
    src/main.c
    src/graphics/renderer.c
    src/graphics/shader.c
    glad/src/gl.c
)

target_include_directories(topo
    PRIVATE
        include
        glad/include
)

target_link_libraries(topo
    PRIVATE
        glfw
        GL
)
