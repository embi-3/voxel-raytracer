#include "ray.hpp"
#include "aabb.hpp"
#include <limits>
#include <iostream>

static const num EPSILON = std::numeric_limits<num>::epsilon();

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

    IntersectionList geometry::Ray::traverse(VoxelGrid grid) {
        IntersectionList objects = IntersectionList(dir);

        // ! DEBUG
        // std::cout << "[t] Traversing: " << dir << "\n";
        Vec3 pos;
        Vec3 tmax;
        Vec3 tdelta = Vec3(dir.x == 0 ? std::numeric_limits<num>::infinity() : grid.scale.x * inv_dir.x,
                           dir.y == 0 ? std::numeric_limits<num>::infinity() : grid.scale.y * inv_dir.y,
                           dir.z == 0 ? std::numeric_limits<num>::infinity() : grid.scale.z * inv_dir.z);
        num tcur = 0;
        Coordinate coords;

        // ! DEBUG
        // std::cout << "[t] Traversal initialised...\n";

        // Check if the ray is already in the voxel grid.
        if (grid.contains(origin)) {
            std::cout << "[t] Already in grid!\n";
            pos = origin;
            tmax = tdelta;
        }
        else {
            Interval interval = intersection(grid.bounding_box);
            // ! DEBUG
            // std::cout << "[i] Interval: [" << interval.min << ", " << interval.max << "]\n";
            if (interval.is_valid()) {
                pos = at(interval.min);
                tcur = interval.min;
                tmax = Vec3(interval.min);
                tmax += tdelta;
            } else {
                // If the ray doesn't hit the bounding box, return an empty list.
                // ! DEBUG
                std::cerr << "Doesn't intersect!\n";
                return objects;
            }
        }

        coords = grid.get_coords(pos);
        // ! DEBUG
        // std::cerr << "Coords: " << coords << "\n";

        // Iteratively find the next voxel using floating-point comparisons.
        while (grid.contains(at(tcur))) {
            // ! DEBUG
            // std::cerr << "[c] " << coords << "\n";
            optional<Voxel> intersect = grid.at(coords);
            if (intersect.has_value() && intersect.value().is_opaque()) {
                // ! DEBUG
                // std::cerr << "[i] Intersection at: " << at(tcur) << "\n";
                objects.push_back(Intersection(intersect.value(), tcur));
            }

            // Update the Amanatides-Woo algorithm to handle diagonals.
            if (tmax.x <= tmax.y && tmax.x <= tmax.z) {
                // ! DEBUG
                // std::cerr << "min x\n";
                tcur = tmax.x;
                tmax.x += tdelta.x;
                coords.x = static_cast<unsigned int>(std::max(static_cast<int>(coords.x) + orientation.x, 0));
            }

            if (tmax.y <= tmax.x && tmax.y <= tmax.z) {
                // ! DEBUG
                // std::cerr << "min y\n";
                tcur = tmax.y;
                tmax.y += tdelta.y;
                coords.y = static_cast<unsigned int>(std::max(static_cast<int>(coords.y) + orientation.y, 0));
            }

            if (tmax.z <= tmax.x && tmax.z <= tmax.y) {
                // ! DEBUG
                // std::cerr << "min z\n";
                tcur = tmax.z;
                tmax.z += tdelta.z;
                coords.z = static_cast<unsigned int>(std::max(static_cast<int>(coords.z) + orientation.z, 0));
            }
        }
 
        return objects;
    }

    IntersectionList geometry::Ray::traverse(Scene scene) {
        auto grids = scene.grids;

        IntersectionList objects = IntersectionList(dir);
        // ! DEBUG
        // int count = 0;
        for (std::vector<VoxelGrid>::iterator grid = grids.begin(); grid != grids.end(); ++grid) {
            // ! DEBUG
            // std::cout << count++ << "\n";
            // Avoid redundant traversals by checking bounding box intersection first.
            // std::cout << intersection((*grid).bounding_box) << ": ";
            
            if (intersects((*grid).bounding_box)) {
                // ! DEBUG
                // std::cout << "intersection\n";
                IntersectionList intersections = traverse(*grid);
                
                // ! DEBUG
                // std::cerr << "[i] " << intersections.items.size() << " intersection(s) with " << dir << "\n";
                // Insert all the new intersections into the object list.
                objects.insert(objects.end(), intersections.begin(), intersections.end());
            }
        }

        return objects;
    }

    Interval geometry::Ray::intersection(AABB bounding_box) {
        num t_min = 0;
        num t_max = std::numeric_limits<num>::infinity();

        num x_min = (bounding_box.min.x - origin.x) * inv_dir.x;
        num x_max = (bounding_box.max.x - origin.x) * inv_dir.x;
        num y_min = (bounding_box.min.y - origin.y) * inv_dir.y;
        num y_max = (bounding_box.max.y - origin.y) * inv_dir.y;
        num z_min = (bounding_box.min.z - origin.z) * inv_dir.z;
        num z_max = (bounding_box.max.z - origin.z) * inv_dir.z;

        t_min = std::max(x_min, t_min);
        t_min = std::max(y_min, t_min);
        t_min = std::max(z_min, t_min);

        t_max = std::min(x_max, t_max);
        t_max = std::min(y_max, t_max);
        t_max = std::min(z_max, t_max);

        return Interval(t_min, t_max);
    }

    bool geometry::Ray::intersects(AABB bounding_box) {
        return intersection(bounding_box).is_valid();
    }
} // namespace geometry