#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>

#include "../src/simulation.hpp"
#include "../src/initial_conditions.hpp"
#include "../src/octree_node.hpp"

void test_naive(int N, int steps, FILE* csv_file){
    const double theta = 0.5; // Threshold for Barnes-Hut

    printf("Starting simulation %i bodies %i steps \n", N, steps);
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
    Simulation sim(bodies);

    auto treebuild_end = std::chrono::high_resolution_clock::now();
    printf("Create tree for %i bodies took %li seconds \n", N, std::chrono::duration_cast<std::chrono::seconds>(treebuild_end - start).count());
    // 4. Run both simulations side-by-side
    for (int step = 0; step < steps; ++step) {
        // Advance one step 
        sim.step(); // Barnes-Hut
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    printf("Simulated %i steps %i bodies took %li seconds \n", N, steps, duration);
    // Write csv
    fprintf(csv_file, "%i,%li\n", N, duration);
}

int main(){
    // Test Barnes Hut with increasing N to evaluate complexity
    const int steps = 1000;
    const char* result_file = "../unit_test/results/naive.csv";
    FILE* file = fopen(result_file, "w");
    fprintf(file, "N,time(secs)\n");
    for (int i=1; i<=100; i++){
        int N = 10*i;
        printf("Starting simulation %i bodies %i steps \n", N, steps);
        test_naive(N, steps, file);
    }
    fclose(file);
    return 0;
}   

