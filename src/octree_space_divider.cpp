#include "octree_space_divider.hpp"

OctreeSpaceDivider::~OctreeSpaceDivider() {
    delete root;
    root = nullptr;
}

void OctreeSpaceDivider::build(std::vector<Body>& bodies) {
    if (!root) return;

    // Reuse the code from buildOctree
    root = buildOctree(bodies);
}

void OctreeSpaceDivider::computeForce(Body& b, double theta, ForceField* forcefield) const {
    if (!root) return;

    // Reuse the code from computeForceFromNote
    computeForceFromNode(root, b, theta, forcefield);
}