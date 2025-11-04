#ifndef VOXELGRID_H
#define VOXELGRID_H

#include "../common.hpp"
#include "voxel.hpp"
#include "aabb.hpp"
#include <cmath>
#include <iostream>

#include <vector>

#define MAX_WORLD_SIZE 100

namespace geometry {
    class VoxelGrid {
    public:
        Vec3 scale = Vec3(1); // Size of a voxel in 3D space
        Coordinate size = Coordinate(32); // Default world dimensions

        Vec3 origin = Vec3();
        std::vector<Voxel> world;

        // Precompute the bounding box of the grid for performance.
        AABB bounding_box;

        explicit VoxelGrid() {
            world.reserve(size.x * size.y * size.z); // TODO: Change the way voxels are stored.
            
            Vec3 min_bounds = origin - Vec3(0.5 * scale.x, 0.5 * scale.y, 0.5 * scale.z);
            Vec3 max_bounds = origin + Vec3((size.x - 0.5) * scale.x, (size.y - 0.5) * scale.y, (size.z - 0.5) * scale.z);
            bounding_box = AABB(min_bounds, max_bounds);
        }

        explicit VoxelGrid(unsigned int world_size) {
            size = Coordinate(world_size);
            VoxelGrid();
        }

        explicit VoxelGrid(unsigned int x, unsigned int y, unsigned int z) {
            size = Coordinate(x, y, z);
            VoxelGrid();
        }

        Voxel get_voxel([[maybe_unused]] Coordinate coords) {
            // TODO: Implement a function for accessing voxels in the grid by coordinates.
            return Voxel();
        }

        Voxel get_voxel([[maybe_unused]] Vec3 pos) {
            // TODO: Implement a function for accessing voxels in the grid by position.
            return Voxel();
        }

        Coordinate get_coords([[maybe_unused]] Vec3 pos) {
            return Coordinate();
        }

        void set_voxel([[maybe_unused]] int x, [[maybe_unused]] int y, [[maybe_unused]] int z) {
            // Throw an error if the coordinates are invalid.
        }

        bool contains(Vec3 position) {
            return position.x - origin.x <= (size.x - 0.5) * scale.x
                && position.y - origin.y <= (size.y - 0.5) * scale.y
                && position.z - origin.z <= (size.z - 0.5) * scale.z
                && origin.x - position.x >= 0.5 * scale.x
                && origin.y - position.y >= 0.5 * scale.y
                && origin.z - position.z >= 0.5 * scale.z;
        }
    };
} // namespace geometry

#endif // VOXELGRID_H