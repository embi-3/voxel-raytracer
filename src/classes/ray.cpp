#include "ray.hpp"
#include "aabb.hpp"

namespace geometry
{
    // Intersection geometry::Ray::traverse(VoxelGrid grid) {
    //     // Check if the ray is already in the voxel grid.
    //     if (grid.contains(origin)) {
    //         return Intersection(grid.get_voxel_at_coords(origin));
    //     } else {
    //         // ? perhaps we can also precompute the bounding box for the VoxelGrid during initialisation?
    //         AABB bounding_box = AABB(grid.min_bounds, grid.max_bounds);
    //         Interval interval = bounding_box.intersection(*this);
    //         if (interval.isValid) {
    //             // ! This code isn't great - improve the clarity later.
    //             return Intersection(grid.get_voxel_at_coords((*this).at(interval.start)), interval.start);
    //         } else {
    //             // ! Discuss a better way to represent an invalid type / no result.
    //             return Intersection::invalid();
    //         }
    //     }

    //     // ? What do we return if there is no intersection?
    //     // ? At times like this I wish for an Option type.
    // }

    // ? Perhaps this can be extended to work for multiple VoxelGrids?
    IntersectionList geometry::Ray::traverse(VoxelGrid grid) {
        IntersectionList objects = {};

        Vec3 pos;
        Vec3 tmax;
        Vec3 tdelta = Vec3(grid.scale / dir.x, grid.scale / dir.y, grid.scale / dir.z);
        Vec3 tstep = orientation; // ? Is (*this) required here?
        int x = 0;
        int y = 0;
        int z = 0;

        // Check if the ray is already in the voxel grid.
        if (grid.contains(origin)) {
            pos = origin;
            tmax = tdelta;
        } else {
            AABB bounding_box = AABB(grid.min_bounds, grid.max_bounds);
            Interval interval = bounding_box.intersection(*this);
            if (interval.isValid) {
                pos = at(interval.start);
                tmax = Vec3(interval.start);
                tmax += tdelta;
            } else {
                // If the ray doesn't hit the bounding box, return an empty list.
                return objects;
            }
        }

        grid.get_coords(pos, &x, &y, &z);

        // Iteratively find the next voxel using floating-point comparisons.
        while (grid.contains(pos)) {
            // Update the Amanatides-Woo algorithm to handle diagonals.
            if (tmax.x <= tmax.y && tmax.x <= tmax.z) {
                tmax.x += tdelta.x;
                x += orientation.x;
            }
            
            if (tmax.y <= tmax.x && tmax.y <= tmax.z) {
                tmax.y += tdelta.y;
                y += orientation.y;
            }
            
            if (tmax.z <= tmax.x && tmax.z <= tmax.y) {
                tmax.z += tdelta.z;
                z += orientation.z;
            }
        }
    }
} // namespace geometry