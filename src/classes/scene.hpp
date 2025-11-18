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
    };
} // namespace geometry
#endif // SCENE_H