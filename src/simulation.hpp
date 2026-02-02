#pragma once

#include "body.hpp"
#include "octree_space_divider.hpp"

#include <vector>
#include <fstream>
#include <memory>

#include "forces.h"
#include "octree_node.hpp"
#include "timer.h"
/*
Handle the time evolution of the system of bodies
*/
class Simulation {
public:
    ForceField* forcefield;
    Simulation(std::vector<Body> bodies, double dt_, const char* csv_filepath = nullptr, ForceField* forcefield=nullptr, const char* name="Experiment");

    const char* name;
    
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
    double dt;
    std::vector<Body> bodies;
    FILE* csv_file = nullptr;

    std::unique_ptr<SpaceDivider> spaceDivider; // For Barnes-Hut

    void computeForcesNaive(); // Naive O(N^2) force computation
    void computeForcesBarnesHut(double theta); // Compute gravitational forces using the Barnes-Hut octree

    // Timer objects for measuring computation time
    Timer build_tree_timer = Timer("Build Tree");
    Timer compute_force_timer = Timer("Compute Force"); 
    Timer file_write_timer = Timer("Write CSV");
};