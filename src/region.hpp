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

    /*
    Check if a point is inside this region
    */
    bool contains(const Vec3& position) const {
        return (position.x >= center.x - halfSize && position.x <= center.x + halfSize) &&
               (position.y >= center.y - halfSize && position.y <= center.y + halfSize) &&
               (position.z >= center.z - halfSize && position.z <= center.z + halfSize);
    }

    /*
    Return the cubic region of the child octant with index 0 -> 7

    Each child cube is located either to the left or right in x, either bottom or top in y, and either back or front in z. For each child, we need to know: Should we
    move the child center in the - direction or + direction along each axis?

    We use the following convention for the 8 child cubes:

              z+ (front)
              ^
        6-----7
       /|    /|
      2-----3 |
      | 4---|-5 --> x+ (right)
      |/    |/
      0-----1
     /
     v
     y+ (top)
    
    This correspond to the ordering:
        Child 0: (-1, -1, -1)
        Child 1: (1, -1, -1)
        Child 2: (-1, 1, -1)
        Child 3: (1, 1, -1)
        Child 4: (-1, -1, 1)
        Child 5: (1, -1, 1)
        Child 6: (-1, 1, 1)
        Child 7: (1, 1, 1)
    */
    Region childRegion(int index) const {
        // The child cube is half the size of the parent in each dimension
        double childHalfSize = halfSize * 0.5;

        // Define the 8 possible directional shifts in a lookup table
        static const int dx[8] = {-1, 1, -1, 1, -1, 1, -1, 1};
        static const int dy[8] = {-1, -1, 1, 1, -1, -1, 1, 1};
        static const int dz[8] = {-1, -1, -1, -1, 1, 1, 1, 1};

        // Start with the parent's center
        Vec3 childCenter = center;

        // Move the center by childHalfSize in the correct direction for this octant
        childCenter.x += dx[index] * childHalfSize;
        childCenter.y += dy[index] * childHalfSize;
        childCenter.z += dz[index] * childHalfSize;

        // Create and return the child's region
        return Region(childCenter, childHalfSize);
    }
};
