#pragma once

#include <vector>
#include "vec3.hpp"
#include "body.hpp"
#include "region.hpp"

/*
Each OctreeNode represents a region of space (a cube)

A node is either a leaf with 0 or 1 bodies, or an internal node with 8 children and no direct body
*/
class OctreeNode {
public:
    Region bounds; // The cube that this node covers
    Body* body; // Pointer to body, only used for leaf with 1 body
    OctreeNode* children[8]; // Eight children, nullptr if not present

    double mass; // Total mass of all bodies in subtree
    Vec3 centerOfMass; // Center of mass of all bodies in subtree

    /*
    Initialize all children to nullptr
    */
    OctreeNode(const Region& region) : bounds(region), body(nullptr), mass(0.0), centerOfMass{0.0, 0.0, 0.0} {
       for (int i = 0; i < 8; ++i) {
            children[i] = nullptr; 
       }
    }

    /*
    Check if this node is a leaf node
    */
    bool isLeaf() const {
        // Node is leaf is all children are nullptr
        for (int i = 0; i < 8; ++i) {
            if(children[i] != nullptr) {
                return false;
            }
        }
        return true;
    }

    void insert(Body* b);
};

/*
Compute a cubic region that contains all bodies

This will be used as the bounds for the root octree node
*/
Region computeRootRegion(const std::vector<Body>& bodies);

/*
Decide which child octant 0 -> 7 of a region contains a given body (here we only are about the position)

We'll use this inside OctreeNode::insert to choose the correct child
*/
int getChildIndex(const Region& region, const Vec3& point);

/*
Build an octree for the given bodies & return a pointer to the root node
*/
OctreeNode* buildOctree(std::vector<Body>& bodies);

/*
Helpver function to compute gravitational force from 'node' on body 'b'

theta is the threshold for s / d, G is the gravitational constant
*/
void computeForceFromNode(const OctreeNode* node, Body& b, double theta, double G);