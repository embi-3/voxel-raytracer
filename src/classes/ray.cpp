#include "ray.hpp"
#include "aabb.hpp"
#include "intersection.hpp"
#include <iostream>
#include <limits>

namespace geometry {
    static const num infinity = std::numeric_limits<num>::infinity();

    IntersectionList geometry::Ray::traverse(VoxelGrid grid) {
        IntersectionList objects = IntersectionList(dir);

        Direction normal = Direction(NONE);

        // ! DEBUG
        std::cout << "[t] Traversing: " << dir << "\n";
        Vec3 pos;
        Vec3 tmax;
        Vec3 tdelta = Vec3(dir.x == 0 ? infinity : fabs(grid.scale.x * inv_dir.x),
                           dir.y == 0 ? infinity : fabs(grid.scale.y * inv_dir.y),
                           dir.z == 0 ? infinity : fabs(grid.scale.z * inv_dir.z));
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
            Vec3 min;
            Vec3 max;
            Interval interval = intersection(grid.bounding_box, normal, min, max);

            // ! DEBUG
            std::cout << "[t] Interval: [" << interval.min << ", " << interval.max << "]\n";
            if (interval.is_valid()) {
                pos = at(interval.min);
                tcur = interval.min;
                
                // Calculate the first t which intersects with each coordinate plane.
                // If it's infinity, don't bother.
                // ! Double check the order of multiplications / divisions is correct!
                num first_x = (max.x == infinity ? infinity : std::max(min.x, max.x - (floor((max.x - interval.min) * dir.x / grid.scale.x) * grid.scale.x * inv_dir.x)));
                num first_y = (max.y == infinity ? infinity : std::max(min.y, max.y - (floor((max.y - interval.min) * dir.y / grid.scale.y) * grid.scale.y * inv_dir.y)));
                num first_z = (max.z == infinity ? infinity : std::max(min.z, max.z - (floor((max.z - interval.min) * dir.z / grid.scale.z) * grid.scale.z * inv_dir.z)));
                
                // ! DEBUG
                // std::cerr << max.z * inv_dir.x

                tmax = Vec3(first_x, first_y, first_z);

                std::cerr << "pos: " << pos << ", tcur: " << tcur << ", tmax: " << tmax << "\n";

                if (normal.is_x()) {
                    tmax.x += tdelta.x;
                }
                if (normal.is_y()) {
                    tmax.y += tdelta.y;
                }
                if (normal.is_z()) {
                    tmax.z += tdelta.z;
                } else {
                    tmax += tdelta;
                }
            }
            else {
                // If the ray doesn't hit the bounding box, return an empty list.
                // ! DEBUG
                std::cerr << "Doesn't intersect!\n";
                return objects;
            }
        }

        coords = grid.get_coords(pos, orientation);
        // ! DEBUG
        std::cerr << "Starting coords: " << coords << "\n";

        // Iteratively find the next voxel using floating-point comparisons.
        while (grid.contains(coords)) {
            // ! DEBUG
            std::cerr << "[c] " << coords << ": [" << tcur << "] " << at(tcur) << " " << normal << " => " << grid.get_coords(at(tcur), orientation) << "\n";
            // std::cerr << "[c] tmax: " << tmax << "\n";
            Voxel intersect = grid.get_voxel(coords);
            if (intersect.is_opaque()) {
                // ! DEBUG
                // ? This debug buffers when writing to a file for some reason?
                std::cerr << "[i] ray: " << dir << ", dist: " << tcur * dir.length() << ", pos: " << at(tcur) << ", coords: " << grid.get_coords(at(tcur), orientation) << ", normal: " << normal << "\n";
                // std::cerr << "[i] ray: " << dir << ", coords: " << at(tcur) << ", normal: " << normal << "\n";
                objects.push_back(Intersection(intersect, tcur * dir.length(), normal));

                // ! TEMP
                break;
            }

            // Create a temporary variable so any traversal updates don't affect the current iteration.
            Vec3 tmax_temp = tmax;

            // ! DEBUG
            std::cerr << "tmax: " << tmax_temp << "\n";

            normal = Direction(NONE);

            // Update the Amanatides-Woo algorithm to handle diagonals.
            if (tmax_temp.x <= tmax_temp.y && tmax_temp.x <= tmax_temp.z) {
                // ! DEBUG
                // std::cerr << "min x\n";
                tcur = tmax_temp.x;
                tmax.x += tdelta.x;
                coords.x += orientation.x;
                if (orientation.x == 1) {
                    normal.dir += X_POS;
                } else {
                    normal.dir += X_NEG;
                }
            }

            if (tmax_temp.y <= tmax_temp.x && tmax_temp.y <= tmax_temp.z) {
                // ! DEBUG
                // std::cerr << "min y\n";
                tcur = tmax_temp.y;
                tmax.y += tdelta.y;
                coords.y += orientation.y;

                if (orientation.y == 1) {
                    normal.dir += Y_POS;
                } else {
                    normal.dir += Y_NEG;
                }
            }

            if (tmax_temp.z <= tmax_temp.x && tmax_temp.z <= tmax_temp.y) {
                // ! DEBUG
                // std::cerr << "min z\n";
                tcur = tmax_temp.z;
                tmax.z += tdelta.z;
                coords.z += orientation.z;

                if (orientation.z == 1) {
                    normal.dir += Z_POS;
                } else {
                    normal.dir += Z_NEG;
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
        Direction dummy;
        return intersection(bounding_box, dummy);
    }

    Interval geometry::Ray::intersection(AABB bounding_box, Direction &normal) {
        Vec3 dummy;
        return intersection(bounding_box, normal, dummy, dummy);
    }

    Interval geometry::Ray::intersection(AABB bounding_box, Direction &normal, Vec3 &tmin, Vec3 &tmax) {
        num t_min = 0;
        num t_max = infinity;

        num x_min = ((orientation.x == 1 ? bounding_box.min.x : bounding_box.max.x) - origin.x) * inv_dir.x;
        num x_max = ((orientation.x == 1 ? bounding_box.max.x : bounding_box.min.x) - origin.x) * inv_dir.x;
        num y_min = ((orientation.y == 1 ? bounding_box.min.y : bounding_box.max.y) - origin.y) * inv_dir.y;
        num y_max = ((orientation.y == 1 ? bounding_box.max.y : bounding_box.min.y) - origin.y) * inv_dir.y;
        num z_min = ((orientation.z == 1 ? bounding_box.min.z : bounding_box.max.z) - origin.z) * inv_dir.z;
        num z_max = ((orientation.z == 1 ? bounding_box.max.z : bounding_box.min.z) - origin.z) * inv_dir.z;

        std::cout << "x: " << Interval(x_min, x_max) << ", y: " << Interval(y_min, y_max) << ", z: " << Interval(z_min, z_max) << "\n";

        t_min = std::max(x_min, t_min);
        t_min = std::max(y_min, t_min);
        t_min = std::max(z_min, t_min);

        t_max = std::min(x_max, t_max);
        t_max = std::min(y_max, t_max);
        t_max = std::min(z_max, t_max);

        normal = Direction(NONE);

        if (t_min == x_min) {
            normal.dir = static_cast<uint8_t>(normal.dir + (orientation.x == 1 ? X_POS : X_NEG));
        }
        if (t_min == y_min) {
            normal.dir = static_cast<uint8_t>(normal.dir + (orientation.y == 1 ? Y_POS : Y_NEG));
        }
        if (t_min == z_min) {
            normal.dir = static_cast<uint8_t>(normal.dir + (orientation.z == 1 ? Z_POS : Z_NEG));
        }

        tmin = Vec3(x_min, y_min, z_min);
        tmax = Vec3(x_max, y_max, z_max);

        return Interval(t_min, t_max);
    }

    bool geometry::Ray::intersects(AABB bounding_box) {
        return intersection(bounding_box).is_valid();
    }
} // namespace geometry