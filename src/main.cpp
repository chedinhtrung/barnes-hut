#include <iostream>
#include <vector>
#include <fstream>

#include "simulation.hpp"
#include "initial_conditions.hpp"
#include "octree_node.hpp"

int main() {
    // 1. Simulation settings
    const int N = 100; // Number of bodies
    const double G = 1.0; // Gravitational constant, scaled units, don't use the real G here because it might lead to underflow
    const double dt = 0.002; // Timestep size
    const int steps = 2000; // Number of timesteps to simulate
    const double theta = 0.5; // Threshold for Barnes-Hut

    // 2. Generate initial bodies
    std::vector<Body> bodies_bh = randomInitialization(
        N,
        1.0, 10.0, // Mass range: [1, 10]
        10.0, // Position range: [-10, 10]
        0.1, // Velocity range: [-0.1, 0.1]
        1234 // Random seed
    );
    // 3. Create two Simulation objects with the same initial state
    Simulation simBH(bodies_bh, G);

    // Open csv file
    FILE* barneshut_csv = fopen("../results/barnes-hut.csv", "w");

    const auto& bodiesBH = simBH.getBodies();
    // Write the head of the dataframe
    for (const Body& body : bodiesBH){
        fprintf(barneshut_csv, "step, time, body, m, x, y, z, vx, vy, vz");
    }

    // 4. Run both simulations side-by-side
    for (int step = 0; step < steps; ++step) {
        // Advance one step with each method
        simBH.stepBarnesHut(dt, theta); // Barnes-Hut

        for (int j=0; j<bodies_bh.size(); j++){
                const Body b = bodiesBH[j];
                                    //"step, time, body, m, x, y, z, vx, vy, vz"
                fprintf(barneshut_csv, "%i, %.4f, %i, %.2f, %.4f, %4f, %4f, %4f, %4f, %4f \n",
                                        step, step*dt, j, b.mass, b.position.x, b.position.y, b.position.z, b.velocity.x, b.velocity.y, b.velocity.z);
        }
        // Dummy logging every 100 steps
        if (step % 100 == 0) {
            const auto& bodiesBH = simBH.getBodies();

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