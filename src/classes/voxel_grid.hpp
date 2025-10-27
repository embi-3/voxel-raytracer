#ifndef VOXELGRID_H
#define VOXELGRID_H

#include "voxel.hpp"
#include <cmath>
#include <iostream>

// #include <vector>
// using std::vector;

#define MAX_WORLD_SIZE 100

namespace geometry {
    class VoxelGrid {
    public:
        // Create a sparse 3D array for storing voxels.
        Voxel*** world;
        Vec3 origin = Vec3();
        int width;
        int length;
        int height;

        explicit VoxelGrid() {
            world = new Voxel**[width];
        }

        explicit VoxelGrid(int world_size) {
            world = new Voxel**[world_size]; // TODO: Change the way voxels are stored.
            width = world_size;
            length = world_size;
            height = world_size;
        }

        Voxel get_voxel([[maybe_unused]] int x, [[maybe_unused]] int y, [[maybe_unused]] int z) {   // remove these when used
            // TODO: Implement an interface for accessing voxels in the grid.
            return Voxel();
        }

        void set_voxel([[maybe_unused]] int x, [[maybe_unused]] int y, [[maybe_unused]] int z) {    // remove these when used
            // Throw an error if the coordinates are invalid.
        }

        bool contains(Vec3 position) {
            return std::abs(position.x - origin.x) <= width && std::abs(position.y - origin.y) <= length
                   && std::abs(position.z - origin.z) <= height;
        }
    };
} // namespace geometry

#endif // VOXELGRID_H