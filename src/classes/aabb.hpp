#ifndef AABB_H
#define AABB_H

#include "../common.hpp"
#include "vec3.hpp"
#include "voxel_grid.hpp"
#include "interval.hpp"
#include "ray.hpp"

namespace geometry {
    // Axis Aligned Bounding Box
    class AABB {
        public:
            Vec3 min;
            Vec3 max;

            explicit constexpr AABB(Vec3 min, Vec3 max) : min(min), max(max) {};

            // Implement the slab method for computing a Ray-AABB intersection.
            Interval intersection(Ray ray);

            bool intersects(Ray ray) {
                return intersection(ray).isValid;
            }
    };
}

<<<<<<< HEAD
#endif // AABB_H
=======
#endif // AABB_H
>>>>>>> 9fec134 (Fix make errors)
