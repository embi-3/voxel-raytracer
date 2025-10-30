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
            Vec3 origin = Vec3();
            num scale = 100;    // size of a voxel, in pixels.
            Voxel ***world;     // TODO: Change the way voxels are stored.
            unsigned int width = 32;     // x-size
            unsigned int length = 32;    // y-size
            unsigned int height = 32;    // z-size

            // Precompute the bounding box of the grid for performance.
            Vec3 min_bounds = origin - Vec3(width * scale, length * scale, height * scale);
            Vec3 max_bounds = origin + Vec3(width * scale, length * scale, height * scale);


            explicit VoxelGrid() {
                world = new Voxel**[width]; // TODO: Change the way voxels are stored.
            }

            explicit VoxelGrid(unsigned int world_size) {
                world = new Voxel**[world_size]; // TODO: Change the way voxels are stored.
                width = world_size;
                length = world_size;
                height = world_size;
            }

            Voxel get_voxel([[maybe_unused]] int x, [[maybe_unused]] int y, [[maybe_unused]] int z) {
                // TODO: Implement a function for accessing voxels in the grid by index.
                return Voxel();
            }

            Voxel get_voxel_at_coords([[maybe_unused]] Vec3 pos) {
                // TODO: Implement a function for accessing voxels in the grid by coordinates.
                return Voxel();
            }

            void set_voxel([[maybe_unused]] int x, [[maybe_unused]] int y, [[maybe_unused]] int z) {
                // Throw an error if the coordinates are invalid.
            }

            bool contains(Vec3 position) {
                return std::abs(position.x - origin.x) <= width
                    && std::abs(position.y - origin.y) <= length
                    && std::abs(position.z - origin.z) <= height;
            }
    };
}

#endif // VOXELGRID_H