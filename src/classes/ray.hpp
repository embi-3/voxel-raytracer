#ifndef RAY_H
#define RAY_H

#include "../common.hpp"
#include "intersection.hpp"
#include "vec3.hpp"
#include "voxel.hpp"
#include "voxel_grid.hpp"
#include <vector>

namespace geometry {
    using IntersectionList = std::vector<Intersection>;

    class Ray {
    public:
        enum Orientation {
            POSITIVE = 1,
            ZERO = 0,
            NEGATIVE = -1,
        };

        Vec3 origin;
        Vec3 dir;
        Coordinate orientation;
        Vec3 inv_dir;

        explicit constexpr Ray(Vec3 origin, Vec3 dir)
        : origin(origin)
        , dir(dir)
        , orientation(Coordinate(x_sign(), y_sign(), z_sign())) {
            inv_dir.x = dir.x == 0 ? std::numeric_limits<num>::infinity() : static_cast<num>(1) / dir.x;
            inv_dir.y = dir.y == 0 ? std::numeric_limits<num>::infinity() : static_cast<num>(1) / dir.y;
            inv_dir.z = dir.z == 0 ? std::numeric_limits<num>::infinity() : static_cast<num>(1) / dir.z;
        }

        IntersectionList traverse(VoxelGrid grid);

        inline Vec3 at(num t) {
            return origin + t * dir;
        }

    private:
        constexpr inline Orientation x_sign() const {
            return get_sign(dir.x);
        }

        constexpr inline Orientation y_sign() const {
            return get_sign(dir.y);
        }

        constexpr inline Orientation z_sign() const {
            return get_sign(dir.z);
        }

        constexpr inline Orientation get_sign(num value) const {
            if (value > 0) {
                return POSITIVE;
            }
            else if (value < 0) {
                return NEGATIVE;
            }
            else {
                return ZERO;
            }
        }
    };
} // namespace geometry

#endif // RAY_H