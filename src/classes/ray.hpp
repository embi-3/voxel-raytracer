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

        enum FaceOrientation {
            X_POS = 1,
            X_NEG = 2,
            Y_POS = 4,
            Y_NEG = 8,
            Z_POS = 16,
            Z_NEG = 32,
        };

        Vec3 origin;
        Vec3 dir;
        Coordinate orientation;

        // Vec3 inv_dir; // TODO: precompute this for better performance

        explicit constexpr Ray(Vec3 origin, Vec3 dir)
        : origin(origin)
        , dir(dir)
        , orientation(Coordinate(x_dir(), y_dir(), z_dir())) {}

        IntersectionList traverse(VoxelGrid grid);

        inline Vec3 at(num t) {
            return origin + t * dir;
        }

    private:
        constexpr inline Orientation x_dir() const {
            return get_dir(dir.x);
        }

        constexpr inline Orientation y_dir() const {
            return get_dir(dir.y);
        }

        constexpr inline Orientation z_dir() const {
            return get_dir(dir.z);
        }

        constexpr inline Orientation get_dir(num value) const {
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