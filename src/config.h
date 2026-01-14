#pragma once

// Simulation parameters
constexpr double G = 1.0;
constexpr std::size_t NUM_BODIES = 100;
constexpr std::size_t NUM_STEPS = 5000;
constexpr double DT = 0.01;
constexpr double THETA = 0.5;

// Initial conditions
constexpr double MASS_MIN = 0.1;
constexpr double MASS_MAX = 0.5;
constexpr double POSITION_RANGE = 30.0;
constexpr double VELOCITY_RANGE = 5.0;
constexpr unsigned int SEED = 1234;

// Output
constexpr const char* NAIVE_CSV = "../results/naive.csv";
constexpr const char* BH_CSV    = "../results/bh.csv";
