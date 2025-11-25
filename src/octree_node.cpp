#include "octree_node.hpp"
#include <algorithm>
#include <limits>

void OctreeNode::insert(Body* b) {

}

Region computeRootRegion(const std::vector<Body>& bodies) {
    double minX =  std::numeric_limits<double>::infinity();
    double minY =  std::numeric_limits<double>::infinity();
    double minZ =  std::numeric_limits<double>::infinity();
    double maxX = -std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();
    double maxZ = -std::numeric_limits<double>::infinity();

    // 1. Find the min & max in each dimension
    for (const Body& body : bodies) {
        const Vec3& p = body.position;

        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;

        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;

        if (p.z < minZ) minZ = p.z;
        if (p.z > maxZ) maxZ = p.z;
    }

    // 2. Compute box center
    double cx = 0.5 * (minX + maxX);
    double cy = 0.5 * (minY + maxY);
    double cz = 0.5 * (minZ + maxZ);

    Vec3 center{cx, cy, cz};

    // 3. Compute the maximum span in all directions
    double spanX = maxX - minX;
    double spanY = maxY - minY;
    double spanZ = maxZ - minZ;

    double maxSpan = std::max({spanX, spanY, spanZ});

    // Avoid zero-size region when all bodies are at the same position
    if (maxSpan == 0.0) {
        maxSpan = 1.0;
    }

    double halfSize = 0.5 * maxSpan;

    return Region{center, halfSize};
}

/*
Decide which child octant 0 -> 7 of the region contains the given point

We use the same convention as in Region::childRegion:

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
*/
int getChildIndex(const Region& region, const Vec3& point) {
    bool right = (point.x >= region.center.x);
    bool top   = (point.y >= region.center.y);
    bool front = (point.z >= region.center.z);

    if (!right && !top && !front) {
        // Left, bottom, back
        return 0;
    }

    if (right && !top && !front) {
        // Right, bottom, back
        return 1;
    }

    if (!right && top && !front) {
        // Left, top, back
        return 2;
    }

    if (right && top && !front) {
        // Right, top, back
        return 3;
    }

    if (!right && !top && front) {
        // Left, bottom, front
        return 4;
    }

    if (right && !top && front) {
        // Right, bottom, front
        return 5;
    }

    if (!right && top && front) {
        // Left, top, front
        return 6;
    }

    // Last remaining case: right && top && front -> index 7
    return 7;
}

