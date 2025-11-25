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
            };
        }
        return true;
    }

    void insert(Body* b);
};