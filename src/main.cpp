#include <iostream>
#include <vector>
#include <fstream>

#include "simulation.hpp"
#include "initial_conditions.hpp"
#include "octree_node.hpp"
#include "config.h"

int main() {
    // 1. Generate initial bodies
    std::vector<Body> bodies_naive = galaxyInitialization(
        NUM_BODIES,
        MASS_MIN, MASS_MAX, // Mass range: [1, 10]
        POSITION_RANGE, // Position range: [-10, 10]
        VELOCITY_RANGE, // Velocity range: [-0.1, 0.1]
        SEED // Random seed
    );

    std::vector<Body> bodies_bh = galaxyInitialization(
        NUM_BODIES,
        MASS_MIN, MASS_MAX, // Mass range: [1, 10]
        POSITION_RANGE, // Position range: [-10, 10]
        VELOCITY_RANGE, // Velocity range: [-0.1, 0.1]
        SEED // Random seed
    );

    std::vector<Body> bodies_bh_r3_gravity = galaxyInitialization(
        NUM_BODIES,
        MASS_MIN, MASS_MAX, // Mass range: [1, 10]
        POSITION_RANGE, // Position range: [-10, 10]
        VELOCITY_RANGE, // Velocity range: [-0.1, 0.1]
        SEED // Random seed
    );

    ClassicalGravity classic_gfield = ClassicalGravity();
    R3Gravity r3_gfield = R3Gravity();
    
    // 2. Create two Simulation objects with the same initial state
    Simulation simBH(bodies_bh, DT, BH_CSV, &classic_gfield, "BH Classic Gravity");
    Simulation simNaive(bodies_naive, DT, NAIVE_CSV, &classic_gfield, "Naive Classic Gravity");

    Simulation simBH_R3Gravity(bodies_bh_r3_gravity, DT, R3Gravity_BH_CSV, &r3_gfield, "BH R3 Gravity");

    // 3. Run both simulations side-by-side
    for (int step = 0; step < NUM_STEPS; ++step) {
        // Advance one step with each method
        simBH.stepBarnesHut(THETA); // Barnes-Hut
        simNaive.step(); // Naive
        simBH_R3Gravity.stepBarnesHut(THETA);

        // Dummy logging every 100 steps
        if (step % 100 == 0) {
            printf("Step: %i \n", step);
        }
    }

    return 0;
}