#ifndef SCENE_H
#define SCENE_H

#include "../common.hpp"
#include "ray.hpp"
#include "voxel_grid.hpp"

#include <memory>
#include <vector>

namespace geometry {
    class Scene {
    public:
        auto push_back(std::unique_ptr<VoxelGrid> grid) {
            grids.push_back(std::move(grid));
        }

        IntersectionList traverse(const Ray& ray) const {
            auto objects = IntersectionList(ray.dir);
            for (const auto& grid : grids) {
                // ! DEBUG
                if (debug) {
                    std::cerr << "Ray: " << ray.dir << "\n";
                }
                if (ray.intersects(grid->bounding_box)) {
                    IntersectionList intersections = grid->fixed_step_traverse(ray);
                    objects.insert(objects.end(), intersections.begin(), intersections.end());
                }
            }

            // ! DEBUG
            if (debug) {
                std::cerr << "[i] " << objects.size() << " intersection(s) with " << ray.dir << "\n";
            }

            return objects;
        }

        size_t mem_usage() const {
            size_t size = sizeof(*this);
            for (const auto& grid : grids) {
                size += grid->mem_usage();
            }

            return size;
        }

    private:
        std::vector<std::unique_ptr<VoxelGrid>> grids;
    };
} // namespace geometry
#endif // SCENE_H