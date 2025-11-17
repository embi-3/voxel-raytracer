#include "ray.hpp"
#include "aabb.hpp"
#include "intersection.hpp"
#include <iostream>
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

    IntersectionList geometry::Ray::traverse(VoxelGrid grid) {
        IntersectionList objects = IntersectionList(dir);

        FaceOrientation normal = FaceOrientation::NONE;

        // ! DEBUG
        std::cout << "[t] Traversing: " << dir << "\n";
        Vec3 pos;
        Vec3 tmax;
        Vec3 tdelta = Vec3(dir.x == 0 ? std::numeric_limits<num>::infinity() : fabs(grid.scale.x * inv_dir.x),
                           dir.y == 0 ? std::numeric_limits<num>::infinity() : fabs(grid.scale.y * inv_dir.y),
                           dir.z == 0 ? std::numeric_limits<num>::infinity() : fabs(grid.scale.z * inv_dir.z));
        num tcur = 0;
        Coordinate coords;

        // ! DEBUG
        // std::cout << "[t] Traversal initialised...\n";
        std::cout << "[t] Delta: " << tdelta << "\n";

        // Check if the ray is already in the voxel grid.
        if (grid.contains(origin)) {
            std::cout << "[t] Already in grid!\n";
            pos = origin;
            tmax = tdelta;
        }
        else {
            Interval interval = intersection(grid.bounding_box, normal, tmax);
            // ! DEBUG
            std::cout << "[t] Interval: [" << interval.min << ", " << interval.max << "]\n";
            if (interval.is_valid()) {
                pos = at(interval.min);
                tcur = interval.min;
                if (normal == FaceOrientation::X_POS || normal == FaceOrientation::X_NEG) {
                    tmax.x += tdelta.x;
                } else if (normal == FaceOrientation::Y_POS || normal == FaceOrientation::Y_NEG) {
                    tmax.y += tdelta.y;
                } else if (normal == FaceOrientation::Z_POS || normal == FaceOrientation::Z_NEG) {
                    tmax.z += tdelta.z;
                } else {
                    tmax += tdelta;
                }
                std::cerr << "pos: " << pos << ", tcur: " << tcur << ", tmax: " << tmax << "\n";
            }
            else {
                // If the ray doesn't hit the bounding box, return an empty list.
                // ! DEBUG
                std::cerr << "Doesn't intersect!\n";
                return objects;
            }
        }

        coords = grid.get_coords(pos);
        // ! DEBUG
        std::cerr << "Coords: " << coords << "\n";

        // Iteratively find the next voxel using floating-point comparisons.
        while (grid.contains(coords)) {
            // ! DEBUG
            std::cerr << "[c] " << coords << ": [" << tcur << "] " << at(tcur) << " " << normal << " - " << grid.get_coords(at(tcur)) << "\n";
            // std::cerr << "[c] tmax: " << tmax << "\n";
            Voxel intersect = grid.get_voxel(coords);
            if (intersect.is_opaque()) {
                // ! DEBUG
                // ? This debug buffers when writing to a file for some reason?
                std::cerr << "[i] ray: " << dir << ", dist: " << tcur * dir.length() << ", pos: " << at(tcur) << ", coords: " << grid.get_coords(at(tcur)) << ", normal: " << normal << "\n";
                // std::cerr << "[i] ray: " << dir << ", coords: " << at(tcur) << ", normal: " << normal << "\n";
                objects.push_back(Intersection(intersect, tcur * dir.length(), normal));

                // ! TEMP
                break;
            }

            // Create a temporary variable so any traversal updates don't affect the current iteration.
            Vec3 tmax_temp = tmax;

            // ! DEBUG
            std::cerr << "tmax: " << tmax_temp << "\n";

            // Update the Amanatides-Woo algorithm to handle diagonals.
            if (tmax_temp.x <= tmax_temp.y && tmax_temp.x <= tmax_temp.z) {
                // ! DEBUG
                // std::cerr << "min x\n";
                tcur = tmax.x;
                tmax.x += tdelta.x;
                coords.x += orientation.x;
                if (orientation.x == 1) {
                    normal = FaceOrientation::X_POS;
                }
                else {
                    normal = FaceOrientation::X_NEG;
                }
            }

            if (tmax_temp.y <= tmax_temp.x && tmax_temp.y <= tmax_temp.z) {
                // ! DEBUG
                // std::cerr << "min y\n";
                tcur = tmax.y;
                tmax.y += tdelta.y;
                coords.y += orientation.y;
                if (orientation.y == 1) {
                    normal = FaceOrientation::Y_POS;
                }
                else {
                    normal = FaceOrientation::Y_NEG;
                }
            }

            if (tmax_temp.z <= tmax_temp.x && tmax_temp.z <= tmax_temp.y) {
                // ! DEBUG
                // std::cerr << "min z\n";
                tcur = tmax.z;
                tmax.z += tdelta.z;
                coords.z += orientation.z;
                if (orientation.z == 1) {
                    normal = FaceOrientation::Z_POS;
                }
                else {
                    normal = FaceOrientation::Z_NEG;
                }
            }
        }

        return objects;
    }

    IntersectionList geometry::Ray::traverse(Scene scene) {
        auto grids = scene.grids;

        IntersectionList objects = IntersectionList(dir);
        // ! DEBUG
        // int count = 0;
        for (std::vector<VoxelGrid*>::iterator grid = grids.begin(); grid != grids.end(); ++grid) {
            // ! DEBUG
            // std::cout << count++ << "\n";
            // Avoid redundant traversals by checking bounding box intersection first.
            // std::cout << dir << ": ";
            // std::cout << intersection((*grid).bounding_box) << " - ";

            if (intersects((**grid).bounding_box)) {
                // ! DEBUG
                // std::cout << "intersects\n";
                IntersectionList intersections = traverse(**grid);

                // ! DEBUG
                // std::cerr << "[i] " << intersections.items.size() << " intersection(s) with " << dir << "\n";
                // Insert all the new intersections into the object list.
                objects.insert(objects.end(), intersections.begin(), intersections.end());
            }
            else {
                // ! DEBUG
                // std::cout << "doesn't intersect\n";
            }
        }

        return objects;
    }
    Interval geometry::Ray::intersection(AABB bounding_box) {
        FaceOrientation dummy;
        return intersection(bounding_box, dummy);
    }

    Interval geometry::Ray::intersection(AABB bounding_box, FaceOrientation &normal) {
        Vec3 dummy;
        return intersection(bounding_box, normal, dummy);
    }

    Interval geometry::Ray::intersection(AABB bounding_box, FaceOrientation &normal, Vec3 &tmin) {
        num t_min = 0;
        num t_max = std::numeric_limits<num>::infinity();

        num x_min = ((orientation.x == 1 ? bounding_box.min.x : bounding_box.max.x) - origin.x) * inv_dir.x;
        num x_max = ((orientation.x == 1 ? bounding_box.max.x : bounding_box.min.x) - origin.x) * inv_dir.x;
        num y_min = ((orientation.y == 1 ? bounding_box.min.y : bounding_box.max.y) - origin.y) * inv_dir.y;
        num y_max = ((orientation.y == 1 ? bounding_box.max.y : bounding_box.min.y) - origin.y) * inv_dir.y;
        num z_min = ((orientation.z == 1 ? bounding_box.min.z : bounding_box.max.z) - origin.z) * inv_dir.z;
        num z_max = ((orientation.z == 1 ? bounding_box.max.z : bounding_box.min.z) - origin.z) * inv_dir.z;

        // std::cout << "x: " << Interval(x_min, x_max) << ", y: " << Interval(y_min, y_max) << ", z: " <<
        // Interval(z_min, z_max) << "\n";

        t_min = std::max(x_min, t_min);
        t_min = std::max(y_min, t_min);
        t_min = std::max(z_min, t_min);

        t_max = std::min(x_max, t_max);
        t_max = std::min(y_max, t_max);
        t_max = std::min(z_max, t_max);

        if (t_min == x_min) {
            normal = (orientation.x == 1 ? FaceOrientation::X_POS : FaceOrientation::X_NEG);
        } else if (t_min == y_min) {
            normal = (orientation.y == 1 ? FaceOrientation::Y_POS : FaceOrientation::Y_NEG);
        } else if (t_min == z_min) {
            normal = (orientation.z == 1 ? FaceOrientation::Z_POS : FaceOrientation::Z_NEG);
        }

        tmin = Vec3(x_min, y_min, z_min);

        return Interval(t_min, t_max);
    }

    bool geometry::Ray::intersects(AABB bounding_box) {
        return intersection(bounding_box).is_valid();
    }
} // namespace geometry