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
        int world_size = 32;

        explicit VoxelGrid() {
            world = new Voxel**[world_size];
        }

        explicit VoxelGrid(int world_size_) {
            world = new Voxel**[world_size_];
            world_size = world_size_;
        }
    };
} // namespace geometry

#endif // VOXELGRID_H