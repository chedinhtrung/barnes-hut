#include <iostream>
#include <vector>

#include "simulation.hpp"
#include "initial_conditions.hpp"

int main() {
    // 1. Simulation settings
    const std::size_t N = 100; // Number of bodies
    const double G = 1.0; // Gravitational constant, scaled units, don't use the real G here because it might lead to underflow
    const double dt = 0.01; // Timestep size
    const int steps = 1000; // Number of timesteps to simulate

    // 2. Generate initial bodies
    std::vector<Body> bodies = randomInitialization(
        N,
        1.0, 10.0, // Mass range: [1, 10]
        10.0, // Position range: [-10, 10]
        0.1, // Velocity range: [-0.1, 0.1]
        1234 // Random seed
    );

    // 3. Create the Simulation object
    Simulation sim(std::move(bodies), G);

    // 4. Simulation
    for (int step = 0; step < steps; ++step) {
        sim.step(dt);

        // Dummy logging
        if (step % 100 == 0) {
            const std::vector<Body>& bodies_ = sim.getBodies();

            if(!bodies_.empty()) {
                const Body& first_body = bodies_[0];

                std::cout << "Step " << step
                          << " | First body position = ("
                          << first_body.position.x << ", "
                          << first_body.position.y << ", "
                          << first_body.position.z << ")\n";
            }
        }
    }

    return 0;
}