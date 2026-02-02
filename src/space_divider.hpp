#pragma once

#include <vector>
#include "body.hpp"
#include "forces.h"

/*
Abstract the space dividing function: Each space dividing function has to do two of the followings:
    1. Build internal spatial structure based on the bodies
    2. Compute and accumulate force contribution onto one body
*/
class SpaceDivider {
public:
    virtual ~SpaceDivider() = default; // Virtual destructor for base class with virtual functions

    // Build internal spatial structure based on bodies -> In this project: Octree
    virtual void build(std::vector<Body>& bodies) = 0;

    // Compute and accumulate force contribution onto one body -> In this project: Barnes-Hut
    virtual void computeForce(Body& b, double theta, ForceField* forcefield) const = 0;
};