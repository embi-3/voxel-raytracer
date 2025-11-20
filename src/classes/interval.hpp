#ifndef INTERVAL_H
#define INTERVAL_H

#include "../common.hpp"

#include <ostream>

namespace geometry {
    class Interval {
    public:
        constexpr explicit Interval(num min, num max) noexcept
        : min(min)
        , max(max) {}

        bool is_valid() const noexcept {
            return min <= max;
        }

        friend std::ostream& operator<<(std::ostream& out, const Interval& i);

        num min;
        num max;
    };

    inline std::ostream& operator<<(std::ostream& out, const Interval& i) {
        return out << "[" << i.min << ", " << i.max << "]";
    }
} // namespace geometry

#endif // INTERVAL_H