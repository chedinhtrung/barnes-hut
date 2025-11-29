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

/* 
returns a vector containing 2N bodies divided into 2 galaxies swirling
*/
std::vector<Body> galaxyInitialization(
    std::size_t N,
    float massMin, float massMax,
    double positionRange,
    double velocityRange,
    unsigned int seed = 1234
);