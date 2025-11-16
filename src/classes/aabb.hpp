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

        bool contains(Vec3 pos) {
            return pos.x >= min.x && pos.x <= max.x && pos.y >= min.y && pos.y <= max.y && pos.z >= min.z
                   && pos.z <= max.z;
        }
    };
} // namespace geometry

#endif // AABB_H
