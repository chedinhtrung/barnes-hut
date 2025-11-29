#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

#include "../src/simulation.hpp"
#include "../src/initial_conditions.hpp"
#include "../src/octree_node.hpp"

int main(){
    // Test 1: simulate 1000 objects for 1000 steps 
    const int N = 10000; // Number of bodies
    const double G = 1.0; // Gravitational constant, scaled units, don't use the real G here because it might lead to underflow
    const double dt = 0.01; // Timestep size
    const int steps = 1000; // Number of timesteps to simulate
    const double theta = 0.5; // Threshold for Barnes-Hut

    printf("Starting naive simulation %i bodies %i steps \n", N, steps);
    // 2. Generate initial bodies
    std::vector<Body> bodies = randomInitialization(
        N,
        1.0, 10.0, // Mass range: [1, 10]
        100.0, // Position range: [-10, 10]
        0.1, // Velocity range: [-0.1, 0.1]
        1234 // Random seed
    );

    auto start = std::chrono::high_resolution_clock::now();
    // 3. Create two Simulation objects with the same initial state
    Simulation simNaive(bodies, G);

    // 4. Run both simulations side-by-side
    for (int step = 0; step < steps; ++step) {
        // Advance one step with each method
        simNaive.step(dt); // Barnes-Hut
    }
    auto end = std::chrono::high_resolution_clock::now();

    printf("Simulated %i steps %i bodies took %li seconds \n", N, steps, std::chrono::duration_cast<std::chrono::seconds>(end - start).count());
    
    return 0;
   
}