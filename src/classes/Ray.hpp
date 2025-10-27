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
            Vec3 dir;
            // Vec3 inv_dir; // precompute this for better performance

            explicit constexpr Ray(Vec3 origin, Vec3 dir) : origin(origin), dir(dir) {}

            Intersection traverse(VoxelGrid grid) {
                // Check if the ray is already in the voxel grid.
                if (grid.contains(origin)) {
                    return Intersection(grid.get_voxel_at_coords(origin));
                }

                // ? What do we return if there is no intersection?
                // ? At times like this I wish for an Option type.
            }

        private:
            num aabb_intersection(VoxelGrid grid) {
                
            }
    };


}

#endif // RAY_H