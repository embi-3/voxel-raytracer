#include "common.hpp"
#include <cmath>

namespace helper {
    bool equals(num lhs, num rhs) {
        return std::fabs(lhs - rhs) < epsilon;
    }

    bool equals_zero(num lhs) {
        return std::fabs(lhs) < epsilon;
    }
} // namespace helper
