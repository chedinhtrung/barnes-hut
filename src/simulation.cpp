#include "simulation.hpp"
#include "octree_node.hpp"
#include <cmath>
#include "forces.h"
#include "timer.h"

Simulation::Simulation(std::vector<Body> bodies, double dt_, const char* csv_filepath, ForceField* forcefield, const char* name): 
                        dt(dt_), bodies(std::move(bodies)), forcefield(forcefield), name(name) {
    spaceDivider = std::make_unique<OctreeSpaceDivider>();

    if (csv_filepath == nullptr){return;}

    // Open csv file
    csv_file = fopen(csv_filepath, "w");

    // Write the head of the dataframe
    fprintf(csv_file, "step,time,body,m,x,y,z,vx,vy,vz\n");
}

// Compute pairwise gravitational forces, naive O(N^2) algorithm
void Simulation::computeForcesNaive() {
    // 1. Reset forces
    for (auto& body : bodies) {
        body.force = Vec3(0.0, 0.0, 0.0);
    }

    std::size_t N = bodies.size(); // Number of bodies
    const double softening = 1e-5; // Avoid division by 0 when calculating gravitational forces

    // 2. Compute pairwise gravitational forces
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = i + 1; j < N; ++j) { // Each body pair (i, j) is processed only once
            Vec3 r = bodies[i].position - bodies[j].position;

            Vec3 fVec(0,0,0);
            // Compute force
            if (!forcefield){continue;}
            switch (forcefield->type){
                case GRAVITY:
                    fVec = forcefield->gravity(bodies[i].mass, bodies[j].mass, r);
            }
           
            bodies[i].force += fVec;
            bodies[j].force -= fVec;
        }
    }
}

void Simulation::step() {
    computeForcesNaive();

    for (auto& body: bodies) {
        Vec3 acceleration = (1.0 / body.mass) * body.force; // a = F / m

        body.velocity += acceleration * dt; // v_new = v_old + a * dt
        body.position += body.velocity * dt; // x_new = x_old + v * dt
    }
    // Write csv
    write_line_csv();
    stepnum++;
}

void Simulation::computeForcesBarnesHut(double theta) {
    // 1. Clear force on all bodies
    for (Body& b : bodies) {
        b.force = Vec3{0.0, 0.0, 0.0};
    }

    build_tree_timer.start();

    // 2. Build the octree and compute forces for each body using Barnes-Hut
    spaceDivider->build(bodies);

    build_tree_timer.stop();

    compute_force_timer.start();

    for (Body& b : bodies) {
        spaceDivider->computeForce(b, theta, forcefield);
    }

    compute_force_timer.stop();
}

void Simulation::stepBarnesHut(double theta) {
    // 1. Compute forces using Barnes-Hut
    computeForcesBarnesHut(theta);

    integration_timer.start();
    // 2. Update velocities + position
    for (Body& b : bodies) {
        b.velocity += (b.force * (1.0 / b.mass)) * dt;
    }

    // 3. Update position
    for (Body& b : bodies) {
      
    }
    integration_timer.stop();

    // 4. Write csv
    file_write_timer.start();
    
    write_line_csv();

    file_write_timer.stop();
    
    stepnum++;
}

void Simulation::write_line_csv(){
    if (csv_file == nullptr){return;}
    for (int j=0; j<bodies.size(); j++){
        const Body b = bodies[j];
                            //"step, time, body, m, x, y, z, vx, vy, vz"
        fprintf(csv_file, "%i,%.4f,%i,%.2f,%.4f,%4f,%4f,%4f,%4f,%4f\n",
                                stepnum, stepnum*dt, j, b.mass, b.position.x, b.position.y, b.position.z, b.velocity.x, b.velocity.y, b.velocity.z);
    }
}

Simulation::~Simulation(){
    if (csv_file != nullptr){
        fclose(csv_file);
    }

    // print the performance timer results  
    printf("Simulation %s,   %i bodies \n\n", name, bodies.size());
    build_tree_timer.print_end_results();
    compute_force_timer.print_end_results();
    file_write_timer.print_end_results();
    integration_timer.print_end_results();
}