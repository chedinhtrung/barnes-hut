#pragma once

#include <vector>
#include "body.hpp"

/*
Return a vector containing N bodies with random mass, position and velocity
*/
std::vector<Body> randomInitialization(
    std::size_t N, // Number of bodies
    double massMin, double massMax,
    double positionRange,
    double velocityRange,
    unsigned int seed = 1234
);
