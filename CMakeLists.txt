cmake_minimum_required(VERSION 3.16)
project(barnes_hut)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(barnes_hut
    src/main.cpp
    src/simulation.cpp
    src/initial_conditions.cpp
)