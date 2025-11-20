#ifndef HELPER_H
#define HELPER_H

#include "common.hpp"
#include <cmath>

namespace helper {
    inline bool equals(num lhs, num rhs) noexcept {
        return std::fabs(lhs - rhs) < epsilon;
    }

    inline bool equals_zero(num lhs) noexcept {
        return std::fabs(lhs) < epsilon;
    }
} // namespace helper

#endif // HELPER_H
