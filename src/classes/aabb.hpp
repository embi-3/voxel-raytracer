#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "../common.hpp"
#include "vec3.hpp"
#include "voxel_grid.hpp"
#include "ray.hpp"
#include "interval.hpp"

namespace geometry {
    // Axis Aligned Bounding Box
    class AABB {
        public:
            Vec3 min;
            Vec3 max;

            explicit constexpr AABB(Vec3 min, Vec3 max) : min(min), max(max) {};

            // Implement the slab method for computing a Ray-AABB intersection.
            Interval intersection(Ray ray) {
                num t_min = -INFINITY; // ? should this be 0 instead?
                num t_max = INFINITY;

                // TODO: Implement precomputation of inverses.
                num x_min = (ray.origin.x - min.x) / ray.dir.x;
                num x_max = (ray.origin.x - max.x) / ray.dir.x;
                num y_min = (ray.origin.y - min.y) / ray.dir.y;
                num y_max = (ray.origin.y - max.y) / ray.dir.y;
                num z_min = (ray.origin.z - min.z) / ray.dir.z;
                num z_max = (ray.origin.z - max.z) / ray.dir.z;

                t_min = std::max(x_min, t_min);
                t_min = std::max(y_min, t_min);
                t_min = std::max(z_min, t_min);

                t_max = std::min(x_max, t_max);
                t_max = std::min(y_max, t_max);
                t_max = std::min(z_max, t_max);

                return Interval(t_min, t_max);
            }

            bool intersects(Ray ray) {
                Interval interval = intersection(ray);
                return interval.isValid;
            }
    };


}

#endif // BOUNDINGBOX_H