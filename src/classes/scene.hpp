#ifndef SCENE_H
#define SCENE_H

#include "../common.hpp"
#include "voxel_grid.hpp"

#include <vector>
#include <memory>

namespace geometry {
    class Scene {
    public:
        std::vector<std::unique_ptr<VoxelGrid>> grids;

        auto push_back(std::unique_ptr<VoxelGrid> grid) {
            grids.push_back(std::move(grid));
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