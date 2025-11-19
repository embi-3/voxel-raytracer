#ifndef HELPER_H
#define HELPER_H

#include "common.hpp"
#include <cmath>

namespace helper {
    inline bool equals(num lhs, num rhs) {
        return std::fabs(lhs - rhs) < epsilon;
    }

    constexpr inline bool equals_zero(num lhs) {
        return std::fabs(lhs) < epsilon;
    }
} // namespace helper

#endif // HELPER_H
