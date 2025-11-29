#include "simulation.hpp"
#include "octree_node.hpp"
#include <cmath>

Simulation::Simulation(std::vector<Body> bodies, double G): bodies(std::move(bodies)), G(G) {}

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
            Vec3 r = bodies[j].position - bodies[i].position;
            double dist2 = norm2(r);
            double dist = std::sqrt(dist2);
            double invDist3 = 1.0 / (dist2 * dist);

            // Compute gravitational force in vector form: fVec = (G * m1 * m2 / |r|^3) * r
            double f = G * bodies[i].mass * bodies[j].mass * invDist3;
            Vec3 fVec = f * r;

            bodies[i].force += fVec;
            bodies[j].force -= fVec;
        }
    }
}

void Simulation::step(double dt) {
    computeForcesNaive();

    for (auto& body: bodies) {
        Vec3 acceleration = (1.0 / body.mass) * body.force; // a = F / m

        body.velocity += acceleration * dt; // v_new = v_old + a * dt
        body.position += body.velocity * dt; // x_new = x_old + v * dt
    }
}

void Simulation::computeForcesBarnesHut(double theta) {
    // 1. Clear force on all bodies
    for (Body& b : bodies) {
        b.force = Vec3{0.0, 0.0, 0.0};
    }

    // 2. Build the octree
    OctreeNode* root = buildOctree(bodies);

    if (root == nullptr) {
        return; // No bodies
    }

    // 3. Compute forces for each body using Barnes-Hut
    for (Body& b : bodies) {
        computeForceFromNode(root, b, theta, G);
    }

    delete(root);
}

void Simulation::stepBarnesHut(double dt, double theta) {
    // 1. Compute forces using Barnes-Hut
    computeForcesBarnesHut(theta);

    // 2. Update velocities
    for (Body& b : bodies) {
        b.velocity += (b.force * (1.0 / b.mass)) * dt;
    }

    // 3. Update position
    for (Body& b : bodies) {
        b.position += b.velocity * dt;
    }
}