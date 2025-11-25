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

    // 2. Generate initial bodies
    std::vector<Body> bodies = randomInitialization(
        N,
        1.0, 10.0, // Mass range: [1, 10]
        10.0, // Position range: [-10, 10]
        0.1, // Velocity range: [-0.1, 0.1]
        1234 // Random seed
    );

    /*
    Compate total mass and center of mass calculated from 1. Direct computation from bodies and 2. Barnes-Hut octree for debugging

    The two methods should give very close results
    */

    // Compute total mass and center of mass directly from bodies
    double directMass = 0.0;
    Vec3 directCOM{0.0, 0.0, 0.0};

    for (const Body& body : bodies) {
        directMass += body.mass;
        directCOM += body.position * body.mass;  // sum(m_i * x_i)
    }

    if (directMass > 0.0) {
        directCOM *= (1.0 / directMass);   // divide by total mass
    }

    // Build the Barnes-Hut octree
    OctreeNode* root = buildOctree(bodies);

    if (root == nullptr) {
        std::cout << "No bodies, octree is empty.\n";
        return 0;
    }

    // Comparison
    std::cout << "========== Octree debug check ==========\n";
    std::cout << "Number of bodies: " << bodies.size() << "\n\n";

    std::cout << "Direct total mass: " << directMass << "\n";
    std::cout << "Tree total mass: " << root->mass << "\n\n";

    std::cout << "Direct center of mass: ("
              << directCOM.x << ", "
              << directCOM.y << ", "
              << directCOM.z << ")\n";

    std::cout << "Tree center of mass: ("
              << root->centerOfMass.x << ", "
              << root->centerOfMass.y << ", "
              << root->centerOfMass.z << ")\n\n";


    // 3. Create the Simulation object
    // Simulation sim(std::move(bodies), G);
    Simulation sim(bodies, G);

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