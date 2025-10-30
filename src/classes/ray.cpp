#include "ray.hpp"
#include "aabb.hpp"

namespace geometry
{
    Intersection geometry::Ray::traverse(VoxelGrid grid) {
        // Check if the ray is already in the voxel grid.
        if (grid.contains(origin)) {
            return Intersection(grid.get_voxel_at_coords(origin));
        } else {
            // ? perhaps we can also precompute the bounding box for the VoxelGrid during initialisation?
            AABB bounding_box = AABB(grid.min_bounds, grid.max_bounds);
            Interval interval = bounding_box.intersection(*this);
            if (interval.isValid) {
                // ! This code isn't great - improve the clarity later.
                return Intersection(grid.get_voxel_at_coords((*this).at(interval.start)), interval.start);
            } else {
                // ! Discuss a better way to represent an invalid type / no result.
                return Intersection::invalid();
            }
        }

        // ? What do we return if there is no intersection?
        // ? At times like this I wish for an Option type.
    }
} // namespace geometry