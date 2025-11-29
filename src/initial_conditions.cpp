#include "initial_conditions.hpp"
#include <random>

std::vector<Body> randomInitialization(
    std::size_t N,
    double massMin, double massMax,
    double positionRange,
    double velocityRange,
    unsigned int seed
) {
    std::vector<Body> bodies;
    bodies.reserve(N);

    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> massRandomDist(massMin, massMax);
    std::uniform_real_distribution<double> posRandomDist(-positionRange, positionRange);
    std::uniform_real_distribution<double> velRandomDist(-velocityRange, velocityRange);

    for (std::size_t i = 0; i < N; ++i) {
        double m = massRandomDist(gen);

        Vec3 position{
            posRandomDist(gen),
            posRandomDist(gen),
            posRandomDist(gen)
        };

        Vec3 velocity{
            velRandomDist(gen),
            velRandomDist(gen),
            velRandomDist(gen)
        };

        bodies.emplace_back(m, position, velocity);
    }

    return bodies;
}

std::vector<Body> galaxyInitialization(
    std::size_t N,
    float massMin, float massMax,
    double positionRange,
    double velocityRange,
    unsigned int seed
)
{
    std::vector<Body> bodies;
    bodies.reserve(2*N);

    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> mass(massMin, massMax);
    std::normal_distribution<double> pos_xy(0, positionRange/4);
    std::normal_distribution<double> pos_z(0, positionRange/4);
    std::normal_distribution<double> speed(0, velocityRange);

    float m_center_A = 1000;
    float m_center_B = 1000;

    for (std::size_t i = 0; i < N; ++i) {
        double mA = mass(gen);
        double mB = mass(gen);

        Vec3 position_A {
            pos_xy(gen) + positionRange,       // galaxy A centered at 20
            pos_xy(gen),
            0.5*pos_z(gen)             // flatten in z to create disc
        };

        Vec3 position_B {
            pos_xy(gen) - positionRange,       // galaxy B centered at -20
            pos_xy(gen),
            0.5*pos_z(gen)         // flatten in z to create disc
        };

        Vec3 velocity_A = cross((position_A - Vec3{positionRange, 0, 0}).normalize(), // galaxy A swirl ccw
                                Vec3 {0.0,0.0,1.0})*(1.0/std::sqrt(norm(position_A - Vec3{positionRange, 0, 0}) + 0.0001))*std::sqrt(m_center_A);
        Vec3 velocity_B = cross((position_B - Vec3{-positionRange, 0, 0}).normalize(), // galaxy A swirl cw
                                Vec3 {0.0,0.0,-1.0})*(1.0/std::sqrt(norm(position_B - Vec3{-positionRange, 0, 0}) + 0.0001))*std::sqrt(m_center_B);


        bodies.emplace_back(mA, position_A, velocity_A);
        bodies.emplace_back(mB, position_B, velocity_B);
    }

    bodies.emplace_back(m_center_A, Vec3{positionRange, 0,0}, Vec3{0,2.5,0});
    bodies.emplace_back(m_center_B, Vec3{-positionRange, 0,0}, Vec3{0,-2.5,0});

    return bodies;
}