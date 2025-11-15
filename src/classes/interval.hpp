#ifndef INTERVAL_H
#define INTERVAL_H

#include "../common.hpp"

namespace geometry {
    class Interval {
    public:
        bool isValid;
        num min;
        num max;

        constexpr explicit Interval(num min, num end)
        : isValid(min <= end)
        , min(min)
        , max(end) {}
    };
} // namespace geometry

#endif // INTERVAL_H