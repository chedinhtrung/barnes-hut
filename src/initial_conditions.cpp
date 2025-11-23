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