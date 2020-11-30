#pragma once
#include <cstdlib>
#include <random>

inline double random_double() {
    //Returns a real random between 0 and 1.0
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max) {
    // Clamps a value between a minimum and maximum
    return (x < min) ? min : ((x > max) ? max : x);
}