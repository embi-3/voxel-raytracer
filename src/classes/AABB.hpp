#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Common.hpp"
#include "Vec3.hpp"
#include "VoxelGrid.hpp"
#include "Ray.hpp"
#include "Interval.hpp"

namespace geometry {
    // Axis Aligned Bounding Box
    class AABB {
        public:
            Vec3 min;
            Vec3 max;
            VoxelGrid& grid; // ? Unsure if I need this here or if I can pass it in as a parameter later.

            explicit AABB(Vec3 min, Vec3 max, VoxelGrid& grid) : min(min), max(max), grid(grid) {};

            // Implement the slab method for computing a Ray-AABB intersection.
            Interval intersection(Ray ray) {
                num t_min = -INFINITY; // ? should this be 0 instead?
                num t_max = INFINITY;
                
                Vec3 grid_min = grid.min_bounds;
                Vec3 grid_max = grid.max_bounds;

                // TODO: Implement precomputation of inverses.
                num x_min = (ray.origin.x - grid_min.x) / ray.dir.x;
                num x_max = (ray.origin.x - grid_max.x) / ray.dir.x;
                num y_min = (ray.origin.y - grid_min.y) / ray.dir.y;
                num y_max = (ray.origin.y - grid_max.y) / ray.dir.y;
                num z_min = (ray.origin.z - grid_min.z) / ray.dir.z;
                num z_max = (ray.origin.z - grid_max.z) / ray.dir.z;

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