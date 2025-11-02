#ifndef RAY_H
#define RAY_H

#include "../common.hpp"
#include "vec3.hpp"
#include "voxel.hpp"
#include "voxel_grid.hpp"
#include "intersection.hpp"
#include <vector>

namespace geometry {
    using IntersectionList = std::vector<Intersection>;

    class Ray {
        public:
            enum Direction {
                POSITIVE = 1,
                ZERO = 0,
                NEGATIVE = -1,
            };

            Vec3 origin;
            Vec3 dir;
            Vec3 orientation;

            // Vec3 inv_dir; // TODO: precompute this for better performance

            explicit constexpr Ray(Vec3 origin, Vec3 dir) : origin(origin), dir(dir), orientation(Vec3(x_dir(), y_dir(), z_dir())) {}

            IntersectionList traverse(VoxelGrid grid);

            Vec3 at(num t) const {
                return origin + t * dir;
            }

        private:
            inline Direction x_dir() {
                return get_dir(dir.x);
            }

            inline Direction y_dir() {
                return get_dir(dir.y);
            }

            inline Direction z_dir() {
                return get_dir(dir.z);
            }
            
            inline Direction get_dir(num value) {
                if (value > 0) {
                    return POSITIVE;
                } else if (value < 0) {
                    return NEGATIVE;
                } else {
                    return ZERO;
                }
            }
    };
} // namespace geometry

#endif // RAY_H