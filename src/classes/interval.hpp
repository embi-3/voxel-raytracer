#ifndef INTERVAL_H
#define INTERVAL_H

#include "../common.hpp"

namespace geometry {
    class Interval {
    public:
        bool isValid;
        num start;
        num end;

        constexpr explicit Interval(num start, num end)
        : isValid(start <= end)
        , start(start)
        , end(end) {}
    };
} // namespace geometry

#endif // INTERVAL_H