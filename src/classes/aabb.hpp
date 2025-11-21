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

        explicit constexpr AABB(Coordinate min, Coordinate max) noexcept
        : min(Vec3{static_cast<double>(min.x), static_cast<double>(min.y), static_cast<double>(min.z)})
        , max(Vec3{static_cast<double>(max.x), static_cast<double>(max.y), static_cast<double>(max.z)}){};

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
