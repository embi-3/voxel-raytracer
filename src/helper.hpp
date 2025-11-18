#ifndef HELPER_H
#define HELPER_H

#include "common.hpp"
#include <cmath>

namespace helper {
    const num PI = 3.141592653589793238462643383279;

    inline bool equals(num lhs, num rhs) {
        return std::fabs(lhs - rhs) < epsilon;
    }

    inline bool equals_zero(num lhs) {
        return std::fabs(lhs) < epsilon;
    }

    inline num deg_to_rad(num deg) {
        return deg * PI / 180.0;
    }

    inline num rad_to_deg(num rad) {
        return rad * 180.0 / PI;
    }
} // namespace helper

#endif // HELPER_H
