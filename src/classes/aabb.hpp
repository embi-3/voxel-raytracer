#ifndef AABB_H
#define AABB_H

#include "../common.hpp"
#include "interval.hpp"
#include "vec3.hpp"

namespace geometry {
    // Axis Aligned Bounding Box
    class AABB {
    public:
        Vec3 min;
        Vec3 max;

        explicit constexpr AABB() = default;
        explicit constexpr AABB(Vec3 min, Vec3 max)
        : min(min)
        , max(max){};
    };
} // namespace geometry

#endif // AABB_H
