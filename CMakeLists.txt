cmake_minimum_required(VERSION 3.16)
project(barnes_hut)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_executable(simulate
    src/main.cpp
    src/simulation.cpp
    src/initial_conditions.cpp
    src/octree_node.cpp
    src/octree_space_divider.cpp
)

add_executable(test_barnes_hut
    src/simulation.cpp
    src/initial_conditions.cpp
    src/octree_node.cpp
    src/octree_space_divider.cpp
    unit_test/test_barnes_hut.cpp
)

add_executable(test_naive
    src/simulation.cpp
    src/initial_conditions.cpp
    src/octree_node.cpp
    src/octree_space_divider.cpp
    unit_test/test_naive.cpp
)

# Link OpenMP to each target
target_link_libraries(simulate PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(test_barnes_hut PRIVATE OpenMP::OpenMP_CXX)
target_link_libraries(test_naive PRIVATE OpenMP::OpenMP_CXX)