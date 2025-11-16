#ifndef SCENE_H
#define SCENE_H

#include "../common.hpp"
#include "voxel_grid.hpp"

#include <vector>

namespace geometry {
    class Scene {
    public:
        std::vector<VoxelGrid*> grids;

        auto push_back(VoxelGrid& grid) {
            grids.push_back(&grid);
        }

        // TODO: Remove these functions if unneeded.
        // void add_grid() {
        //     grids.push_back(VoxelGrid());
        // }

        // void add_grid(unsigned int size, Vec3 origin = Vec3()) {
        //     grids.push_back(VoxelGrid(size, origin));
        // }

        // void add_grid(unsigned int x, unsigned int y, unsigned int z, Vec3 origin = Vec3()) {
        //     grids.push_back(VoxelGrid(x, y, z, origin));
        // }
    };
} // namespace geometry
#endif // SCENE_H