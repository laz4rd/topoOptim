cmake_minimum_required(VERSION 3.20)

project(topoOptim
    VERSION 0.1.0
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(topo
    src/main.c
)
