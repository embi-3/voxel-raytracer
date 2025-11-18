#include "ray.hpp"
#include "../helper.hpp"
#include "aabb.hpp"
#include "intersection.hpp"
#include <iostream>
#include <limits>

using namespace helper;

namespace geometry {
    static const num infinity = std::numeric_limits<num>::infinity();

    IntersectionList Ray::traverse(const VoxelGrid& grid) {
        IntersectionList objects = IntersectionList(dir);

        Vec3 tmax;
        Vec3 tdelta = Vec3(equals_zero(dir.x) ? infinity : fabs(grid.scale.x * inv_dir.x),
                           equals_zero(dir.y) ? infinity : fabs(grid.scale.y * inv_dir.y),
                           equals_zero(dir.z) ? infinity : fabs(grid.scale.z * inv_dir.z));
        num tcur = 0;
        Direction normal = Direction(NONE);
        Coordinate step = orientation.sign();
        Coordinate coords;

        Vec3 min;
        Vec3 max;
        Interval interval = intersection(grid.bounding_box, min, max);

        // ! DEBUG
        std::cout << "[t] Interval: [" << interval.min << ", " << interval.max << "]\n";
        if (interval.is_valid()) {
            tcur = interval.min;

            // Calculate the first t which intersects with each coordinate plane.
            // If it's infinity, don't bother.
            // ! Double check the order of multiplications / divisions is correct!
            num first_x =
                (max.x == infinity
                     ? infinity
                     : std::max(
                         min.x,
                         max.x - (trunc((max.x - interval.min) * dir.x / grid.scale.x) * grid.scale.x * inv_dir.x)));
            num first_y =
                (max.y == infinity
                     ? infinity
                     : std::max(
                         min.y,
                         max.y - (trunc((max.y - interval.min) * dir.y / grid.scale.y) * grid.scale.y * inv_dir.y)));
            num first_z =
                (max.z == infinity
                     ? infinity
                     : std::max(
                         min.z,
                         max.z - (trunc((max.z - interval.min) * dir.z / grid.scale.z) * grid.scale.z * inv_dir.z)));

            tmax = Vec3(first_x, first_y, first_z);

            std::cerr << "pos: " << at(tcur) << ", tcur: " << tcur << ", tmax: " << tmax << "\n";

            if (equals(tmax.x, tcur)) {
                normal += orientation.x();
                tmax.x += tdelta.x;
            }
            if (equals(tmax.y, tcur)) {
                normal += orientation.y();
                tmax.y += tdelta.y;
            }
            if (equals(tmax.z, tcur)) {
                normal += orientation.z();
                tmax.z += tdelta.z;
            }
        }
        else {
            // If the ray doesn't hit the bounding box, return an empty list.
            return objects;
        }

        coords = grid.get_coords(at(tcur), orientation);
        
        // ! DEBUG
        std::cerr << "Starting coords: " << coords << "\n";

        // Iteratively find the next voxel using floating-point comparisons.
        while (grid.contains(coords)) {
            // ! DEBUG
            std::cerr << "[c] " << coords << ": [" << tcur << "] " << at(tcur) << " " << normal << " => " << grid.get_coords(at(tcur), orientation) << "\n";
            
            Voxel voxel = grid.get_voxel(coords);
            if (voxel.is_opaque()) {
                // ! DEBUG
                std::cerr << "[i] ray: " << dir << ", dist: " << tcur * dir.length() << ", pos: " << at(tcur) << ", coords: " << grid.get_coords(at(tcur), orientation) << ", normal: " << normal << "\n";
               
                objects.push_back(Intersection(voxel, tcur * dir.length(), normal));

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
                tcur = tmax_temp.x;
                tmax.x += tdelta.x;
                coords.x += step.x;
                normal += orientation.x();
            }

            if (tmax_temp.y <= tmax_temp.x && tmax_temp.y <= tmax_temp.z) {
                tcur = tmax_temp.y;
                tmax.y += tdelta.y;
                coords.y += step.y;
                normal += orientation.y();
            }

            if (tmax_temp.z <= tmax_temp.x && tmax_temp.z <= tmax_temp.y) {
                tcur = tmax_temp.z;
                tmax.z += tdelta.z;
                coords.z += step.z;
                normal += orientation.z();
            }
        }

        return objects;
    }

    IntersectionList geometry::Ray::traverse(const Scene& scene) {
        IntersectionList objects = IntersectionList(dir);
        for (auto grid = scene.grids.begin(); grid != scene.grids.end(); ++grid) {
            // ! DEBUG
            std::cout << "Ray: " << dir << ", Interval: ";
            std::cout << intersection((**grid).bounding_box) << "\n";

            if (intersects((*grid)->bounding_box)) {
                IntersectionList intersections = traverse(**grid);
                objects.insert(objects.end(), intersections.begin(), intersections.end());
            }
        }

        return objects;
    }

    Interval geometry::Ray::intersection(AABB bounding_box) {
        Vec3 dummy;
        return intersection(bounding_box, dummy, dummy);
    }

    Interval geometry::Ray::intersection(AABB bounding_box, Vec3& tmin, Vec3& tmax) {
        num t_min = 0;
        num t_max = infinity;

        num x_min = (orientation.x().is_none()
                         ? -infinity
                         : ((orientation.is_xpos() ? bounding_box.min.x : bounding_box.max.x) - origin.x) * inv_dir.x);
        num x_max = (orientation.x().is_none()
                         ? infinity
                         : ((orientation.is_xpos() ? bounding_box.max.x : bounding_box.min.x) - origin.x) * inv_dir.x);
        num y_min = (orientation.y().is_none()
                         ? -infinity
                         : ((orientation.is_ypos() ? bounding_box.min.y : bounding_box.max.y) - origin.y) * inv_dir.y);
        num y_max = (orientation.y().is_none()
                         ? infinity
                         : ((orientation.is_ypos() ? bounding_box.max.y : bounding_box.min.y) - origin.y) * inv_dir.y);
        num z_min = (orientation.z().is_none()
                         ? -infinity
                         : ((orientation.is_zpos() ? bounding_box.min.z : bounding_box.max.z) - origin.z) * inv_dir.z);
        num z_max = (orientation.z().is_none()
                         ? infinity
                         : ((orientation.is_zpos() ? bounding_box.max.z : bounding_box.min.z) - origin.z) * inv_dir.z);
                
        // ! DEBUG
        std::cerr << "x: " << Interval(x_min, x_max) << ", y: " << Interval(y_min, y_max) << ", z: " << Interval(z_min, z_max) << "\n";

        t_min = std::max(x_min, t_min);
        t_min = std::max(y_min, t_min);
        t_min = std::max(z_min, t_min);

        t_max = std::min(x_max, t_max);
        t_max = std::min(y_max, t_max);
        t_max = std::min(z_max, t_max);

        tmin = Vec3(x_min, y_min, z_min);
        tmax = Vec3(x_max, y_max, z_max);

        return Interval(t_min, t_max);
    }

    bool geometry::Ray::intersects(AABB bounding_box) {
        return intersection(bounding_box).is_valid();
    }
} // namespace geometry