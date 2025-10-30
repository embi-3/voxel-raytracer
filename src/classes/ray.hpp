#ifndef RAY_H
#define RAY_H

#include "../common.hpp"
#include "vec3.hpp"
#include "voxel.hpp"
#include "voxel_grid.hpp"
#include "intersection.hpp"

namespace geometry {
    class Ray {
        public:
            Vec3 origin;
            Vec3 dir;
            // Vec3 inv_dir; // TODO: precompute this for better performance

            explicit constexpr Ray(Vec3 origin, Vec3 dir) : origin(origin), dir(dir) {}

            Intersection traverse(VoxelGrid grid);

            Vec3 at(num t) const {
                return origin + t * dir;
            }
    };
} // namespace geometry

#endif // RAY_H