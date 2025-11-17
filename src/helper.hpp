#include "common.hpp"
#include <cmath>

namespace helper {
    bool equals(num lhs, num rhs) {
        return std::fabs(lhs - rhs) < epsilon;
    }
} // namespace helper
