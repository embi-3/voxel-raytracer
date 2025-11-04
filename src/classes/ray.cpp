#include "ray.hpp"
#include "aabb.hpp"
#include <limits>

namespace geometry {
    // TODO: Remove deprecated code.
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
        Vec3 tdelta = Vec3(dir.x == 0 ? std::numeric_limits<num>::infinity() : grid.scale.x / dir.x,
                           dir.y == 0 ? std::numeric_limits<num>::infinity() : grid.scale.y / dir.y,
                           dir.z == 0 ? std::numeric_limits<num>::infinity() : grid.scale.z / dir.z);
        num tcur = 0;
        Coordinate coords;

        // Check if the ray is already in the voxel grid.
        if (grid.contains(origin)) {
            pos = origin;
            tmax = tdelta;
        }
        else {
            Interval interval = intersection(grid.bounding_box);
            if (interval.isValid) {
                pos = at(interval.min);
                tmax = Vec3(interval.min);
                tmax += tdelta;
            }
            else {
                // If the ray doesn't hit the bounding box, return an empty list.
                return objects;
            }
        }

        coords = grid.get_coords(pos);

        // Iteratively find the next voxel using floating-point comparisons.
        while (grid.contains(at(tcur))) {
            // Update the Amanatides-Woo algorithm to handle diagonals.
            if (tmax.x <= tmax.y && tmax.x <= tmax.z) {
                tcur = tmax.x;
                tmax.x += tdelta.x;
                coords.x += orientation.x;
            }

            if (tmax.y <= tmax.x && tmax.y <= tmax.z) {
                tcur = tmax.y;
                tmax.y += tdelta.y;
                coords.y += orientation.y;
            }

            if (tmax.z <= tmax.x && tmax.z <= tmax.y) {
                tcur = tmax.z;
                tmax.z += tdelta.z;
                coords.z += orientation.z;
            }

            // TODO: Check that get_voxel actually returns a valid Voxel.
            objects.push_back(Intersection(grid.get_voxel(coords), tcur));
        }

        return objects;
    }

    Interval geometry::Ray::intersection(AABB bounding_box) {
        num t_min = 0;
        num t_max = std::numeric_limits<num>::infinity();

        num x_min = (origin.x - bounding_box.min.x) * inv_dir.x;
        num x_max = (origin.x - bounding_box.max.x) * inv_dir.x;
        num y_min = (origin.y - bounding_box.min.y) * inv_dir.y;
        num y_max = (origin.y - bounding_box.max.y) * inv_dir.y;
        num z_min = (origin.z - bounding_box.min.z) * inv_dir.z;
        num z_max = (origin.z - bounding_box.max.z) * inv_dir.z;

        t_min = std::max(x_min, t_min);
        t_min = std::max(y_min, t_min);
        t_min = std::max(z_min, t_min);

        t_max = std::min(x_max, t_max);
        t_max = std::min(y_max, t_max);
        t_max = std::min(z_max, t_max);

        return Interval(t_min, t_max);
    }

    bool geometry::Ray::intersects(AABB bounding_box) {
        return intersection(bounding_box).isValid;
    }
} // namespace geometry