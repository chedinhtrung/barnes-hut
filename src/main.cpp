#include <iostream>
#include <vector>

#include "simulation.hpp"
#include "initial_conditions.hpp"
#include "octree_node.hpp"

int main() {
    // 1. Simulation settings
    const std::size_t N = 100; // Number of bodies
    const double G = 1.0; // Gravitational constant, scaled units, don't use the real G here because it might lead to underflow
    const double dt = 0.01; // Timestep size
    const int steps = 1000; // Number of timesteps to simulate
    const double theta = 0.5; // Threshold for Barnes-Hut

    // 2. Generate initial bodies
    std::vector<Body> bodies = randomInitialization(
        N,
        1.0, 10.0, // Mass range: [1, 10]
        10.0, // Position range: [-10, 10]
        0.1, // Velocity range: [-0.1, 0.1]
        1234 // Random seed
    );

    // 3. Create two Simulation objects with the same initial state
    Simulation simNaive(bodies, G);
    Simulation simBH(bodies, G);

    // 4. Run both simulations side-by-side
    for (int step = 0; step < steps; ++step) {
        // Advance one step with each method
        simNaive.step(dt); // Brute-force O(N^2)
        simBH.stepBarnesHut(dt, theta); // Barnes-Hut

        // Dummy logging every 100 steps
        if (step % 100 == 0) {
            const auto& bodiesNaive = simNaive.getBodies();
            const auto& bodiesBH = simBH.getBodies();

            if (!bodiesNaive.empty() && !bodiesBH.empty()) {
                const Body& bNaive = bodiesNaive[0];
                const Body& bBH = bodiesBH[0];

                std::cout << "Step " << step << "\n";

                std::cout << "  Naive first body position = ("
                          << bNaive.position.x << ", "
                          << bNaive.position.y << ", "
                          << bNaive.position.z << ")\n";

                std::cout << "  Barnes-Hut first body position = ("
                          << bBH.position.x << ", "
                          << bBH.position.y << ", "
                          << bBH.position.z << ")\n\n";
            }
        }
    }

    return 0;
}