#ifndef INTERVAL_H
#define INTERVAL_H

#include "Common.hpp"

class Interval {
    public:
        bool isValid;
        num start;
        num end;

        constexpr explicit Interval(num start, num end) : isValid(start <= end), start(start), end(end) {}
};

#endif // INTERVAL_H