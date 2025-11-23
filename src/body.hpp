#pragma once

#include "vec3.hpp"

class Body {
public:
    double mass{};

    Vec3 position; // Current position
    Vec3 velocity; // Current velocity
    Vec3 force; // Accumulated force 

    Body() = default;

    Body(double mass_, const Vec3& position_, const Vec3& velocity_) : mass(mass_), position(position_), velocity(velocity_), force(0.0, 0.0, 0.0) {}
};