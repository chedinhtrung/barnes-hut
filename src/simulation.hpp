#pragma once

#include <vector>
#include "body.hpp"
#include "config.h"
#include <fstream>
#include "forces.h"
#include "octree_node.hpp"
/*
Handle the time evolution of the system of bodies
*/
class Simulation {
public:
    ForceField* forcefield;
    Simulation(std::vector<Body> bodies, const char* csv_filepath=nullptr, ForceField* forcefield=nullptr);

    // Advance in time
    void step();
    void stepBarnesHut(double theta); // Advance in time using Barnes-Hut

    // Getter for bodies
    const std::vector<Body>& getBodies() const {
        return bodies;
    }
    void write_line_csv();
    ~Simulation();

private:
    int stepnum = 0;
    std::vector<Body> bodies;
    FILE* csv_file = nullptr;

    void computeForcesNaive(); // Naive O(N^2) force computation
    void computeForcesBarnesHut(double theta); // Compute gravitational forces using the Barnes-Hut octree
    void computeForceFromNode(const OctreeNode* node, Body& b, double theta);
};