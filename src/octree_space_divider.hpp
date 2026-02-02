#pragma once

#include "space_divider.hpp"
#include "octree_node.hpp"

/*
Barnes-Hut space dividing function
*/
class OctreeSpaceDivider : public SpaceDivider {
public:
    OctreeSpaceDivider() = default;
    ~OctreeSpaceDivider() override;

    void build(std::vector<Body>& bodies) override;
    void computeForce(Body& b, double theta, ForceField* forcefield) const override;

private:
    OctreeNode* root = nullptr;
};