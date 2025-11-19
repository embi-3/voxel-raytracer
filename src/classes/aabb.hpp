#ifndef AABB_H
#define AABB_H

#include "../common.hpp"
#include "interval.hpp"
#include "vec3.hpp"

namespace geometry {
    // Axis Aligned Bounding Box
    class AABB {
    public:
        explicit constexpr AABB() noexcept = default;

        explicit constexpr AABB(Vec3 min, Vec3 max) noexcept
        : min(min)
        , max(max){};

        bool contains(Vec3 pos) const noexcept {
            return epsilon >= min.x - pos.x && pos.x - max.x <= epsilon && epsilon >= min.y - pos.y
                   && pos.y - max.y <= epsilon && epsilon >= min.z - pos.z && pos.z - max.z <= epsilon;
        }

        Vec3 min;
        Vec3 max;
    };

    inline std::ostream& operator<<(std::ostream& out, const AABB& aabb) {
        return out << "[" << aabb.min << ", " << aabb.max << "]";
    }
} // namespace geometry

#endif // AABB_H
