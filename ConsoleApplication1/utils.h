#ifndef _UTILS_H_
#define _UTILS_H_

/* Utilitary functions (mostly mathematics)
   Used thorougly the program.
*/

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

inline uint32_t ARGB(uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha)
{
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

//Marsaglia's xorshf generator (Fast Random Function)
static uint32_t s_randX = 123456789;
static uint32_t s_randY = 362436069;
static uint32_t s_randZ = 521288629;

inline uint32_t FastRand(void)
{
    uint32_t t;

    s_randX ^= s_randX << 16;
    s_randX ^= s_randX >> 5;
    s_randX ^= s_randX << 1;

    t = s_randX;
    s_randX = s_randY;
    s_randY = s_randZ;
    s_randZ = t ^ s_randX ^ s_randY;

    return s_randZ;
}

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#endif