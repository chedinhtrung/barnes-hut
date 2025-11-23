#pragma once

#include <vector>
#include "body.hpp"

/*
Handle the time evolution of the system of bodies
*/
class Simulation {
public:
    Simulation(std::vector<Body> bodies, double G);

    // Advance in time
    void step(double dt);

    // Getter for bodies
    const std::vector<Body>& getBodies() const {
        return bodies;
    }

private:
    std::vector<Body> bodies;
    double G; // Universal gravitational constatn

    void computeForcesNaive(); // Naive O(N^2) force computation
};