#ifndef AABB_H
#define AABB_H

#include "../common.hpp"
#include "interval.hpp"
#include "vec3.hpp"

namespace geometry {
    // Axis Aligned Bounding Box
    class AABB {
    public:
        explicit constexpr AABB() = default;

        explicit constexpr AABB(Vec3 min, Vec3 max) noexcept
        : min(min)
        , max(max){};

        bool contains(const Vec3& pos) const noexcept {
            return epsilon >= min.x - pos.x && pos.x - max.x <= epsilon && epsilon >= min.y - pos.y
                   && pos.y - max.y <= epsilon && epsilon >= min.z - pos.z && pos.z - max.z <= epsilon;
        }

        // Maybe make these private?
        Vec3 min;
        Vec3 max;
    };
} // namespace geometry

#endif // AABB_H
