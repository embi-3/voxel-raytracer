#ifndef INTERVAL_H
#define INTERVAL_H

#include "../common.hpp"

#include <ostream>

namespace geometry {
    class Interval {
    public:
        num min;
        num max;

        constexpr explicit Interval(num min, num max)
        : min(min)
        , max(max) {}

        inline bool is_valid() {
            return min <= max;
        }
    };

    inline std::ostream& operator<<(std::ostream& out, const Interval& i) {
        return out << "[" << i.min << ", " << i.max << "]";
    }
} // namespace geometry

#endif // INTERVAL_H