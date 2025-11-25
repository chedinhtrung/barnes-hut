#pragma once

#include "vec3.hpp"

/*
Cubic region in 3D space, represented by its center and half-size
*/
struct Region {
    Vec3 center; // Center of the cube
    double halfSize; // Half the cube edge length

    Region() : center{0.0, 0.0, 0.0}, halfSize(0.0) {}

    Region(const Vec3& c, double h) : center(c), halfSize(h) {}
};
