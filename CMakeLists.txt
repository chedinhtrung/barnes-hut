cmake_minimum_required(VERSION 3.16)
project(barnes-hut)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB HEADERS "include/*.h") 

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(simulator src/main.cpp)