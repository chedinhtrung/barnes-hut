#include <iostream>
#include <vector>
#include <fstream>

#include "simulation.hpp"
#include "initial_conditions.hpp"
#include "octree_node.hpp"

int main() {
    // 1. Simulation settings
    const int N = 100; // Number of bodies
    const int steps = 5000; // Number of timesteps to simulate
    const double theta = 0.5; // Threshold for Barnes-Hut

    // 2. Generate initial bodies
    std::vector<Body> bodies = galaxyInitialization(
        N,
        0.1, 0.5, // Mass range: [1, 10]
        30, // Position range: [-10, 10]
        5, // Velocity range: [-0.1, 0.1]
        1234 // Random seed
    );
    
    // 3. Create two Simulation objects with the same initial state
    Simulation sim(bodies, BH_CSV);
    

    // 4. Run both simulations side-by-side
    for (int step = 0; step < steps; ++step) {
        // Advance one step with each method
        sim.stepBarnesHut(theta); // Barnes-Hut

        // Dummy logging every 100 steps
        if (step % 100 == 0) {
            const auto& bodiesBH = sim.getBodies();

            if (!bodiesBH.empty()) {
                const Body& bBH = bodiesBH[0];

                std::cout << "Step " << step << "\n";

                std::cout << "  Barnes-Hut first body position = ("
                          << bBH.position.x << ", "
                          << bBH.position.y << ", "
                          << bBH.position.z << ")\n\n";
            }
        }
    }

    return 0;
}