#ifndef RAY_H
#define RAY_H

#include "Common.hpp"
#include "Vec3.hpp"
#include "Voxel.hpp"
#include "VoxelGrid.hpp"
#include "Intersection.hpp"

namespace geometry {
    class Ray {
        public:
            Vec3 origin;
            Vec3 direction;

            Intersection traverse(VoxelGrid grid, Voxel voxel) {
                if (grid.contains(origin)) {

                }
            }

        private:
            num aabb_intersection(VoxelGrid grid) {
                Vec3 min = grid.min_bounds;
                Vec3 max = grid.max_bounds;
            }
    };


}

#endif // RAY_H