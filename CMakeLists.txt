cmake_minimum_required(VERSION 3.16)
project(barnes_hut)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(barnes_hut
    src/main.cpp
    src/simulation.cpp
    src/initial_conditions.cpp
    src/vec3.cpp
    src/octree_node.cpp
)

add_executable(test_barnes_hut
    src/simulation.cpp
    src/initial_conditions.cpp
    src/octree_node.cpp
    src/vec3.cpp
    unit_test/test_barnes_hut.cpp
)

add_executable(test_naive
    src/simulation.cpp
    src/initial_conditions.cpp
    src/octree_node.cpp
    src/vec3.cpp
    unit_test/test_naive.cpp
)