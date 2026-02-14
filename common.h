#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <cstdlib>
#include <memory>
#include <limits>
#include <iostream>

// Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degree_to_radians(double degree) {
    return degree * (pi / 180);
}

inline double random_double() {
    // Returns a random real in [0, 1)
    return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Scales to [0, max - min) and bias +min
    return random_double() * (max - min) + min;
}

// Common Headers

#include "ray.h"
#include "vec3.h"
#include "color.h"
#include "interval.h"

#endif